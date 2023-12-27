#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <tuple>
using namespace std;

#include "verilog_driver.hpp"
#include "stdlibrary_TSMC180.h"

using namespace verilog;

#include <numeric>
#include <math.h>

// https://www.geeksforgeeks.org/write-a-c-program-to-calculate-size-of-a-tree/

// A custom parser struct

// Struct: Gate
struct Gate {
	string name;
	string cell;
	unordered_map<string, string> cellpin2net;
	unordered_map<string, string> net2cellpin;
	//int gate_input=0;
};

// Struct: Module
struct Module {
	string name;
	vector<string> ports;
	vector<string> wires;
	vector<string> inputs;
	vector<string> outputs;
	vector<string> verilog_inputs;	// verilog ���� ����Ҷ� ���
	vector<string> verilog_outputs;	// verilog ���� ����� �� ���
	vector<Gate> gates;
};


// A verilog parser compatible with OpenTimer
struct OpenTimerParser : public verilog::ParserVerilogInterface {
private:
	void add_module(std::string&& name);
	void add_port(verilog::Port&& port);
	void add_net(verilog::Net&& net);
	void add_assignment(verilog::Assignment&& ast);
	void add_instance(verilog::Instance&& inst);
	bool isinput(const pair<string, string>& name);
	bool nostrip_isinput(const pair<string, string>& name); //vdd,gnd ������ input ���� Ȯ��
	bool isclock(const unordered_map<string, string>& name);  // clock
	bool isoutput(const unordered_map<string, string>& name);
	bool isOutput_gate(const Gate Output);	// Output gate���� Ȯ���ϱ�

	//vector<unordered_map<string, string>>  nostrip_getchildren(const unordered_map<string, string>& cellpin2net);
	///////////////	output�� �����ϴ� gate vector�� ������
	vector<Gate> Output_Gates;
	void get_output_Module();		// output�� �����ϴ� gate ����
	vector<Gate> Clock_Gates;
	vector<Gate> CLKBUF_gates;		// gate ������ CLKBUF�� �� ����
	void get_clock_Module();		// clock�� �����ϴ� gate ����
	void get_CLKBUF();				// gate ������ CLKBUF�� �� ����

	vector<Gate>Input_Gates;		
	void get_input_Module();		// input�� �����ϴ� gate ����

	///////////////	output���� leaf node������ route ������
	/////////////// Gate�� �ּ��� ������ ����
	void traversal(const Gate& g, int level);

	pair<string, string> get_output_Gate(const unordered_map<string, string>& name);		// output�� �����ϴ� port return
	pair<string, string> get_clock_input_Gate(const unordered_map<string, string>& name);	// CLK port�� �����ϴ� port return

	///////////////  Gate�� celltonet�� ���� Gate�� output�� �ִ� ���� return�Ѵ�.
	vector<Gate> getchildren_Gate(const unordered_map<string, string>& pin2net, pair<string, string> gate_output);
	///////////////  CLK��Ʈ�� �����ϴ� gate�� �ְ�, CLK ��Ʈ�� �����ϴ� ���� ���� CLK ��Ʈ�� output �ݰ� ���ٸ�  children clock gate�̴�.
	vector<Gate> getchildren_CLK_Gate(const Gate& g, pair<string, string> clk_port);
	///////////////
	vector<Gate> getchildren_input2DFF_Gate(const unordered_map<string, string>& pin2net, pair<string, string> input2DFF_port);

	const vector<float> stdlibrary_180nm(string gate_name);	// std cell�� delay, area, gate input�� ����
	vector<pair<string, string>> get_input_Gate(const unordered_map<string, string>& name);	// getInputs : 1���� gate�� �����ϴ� input�� pair<string, string>������ ���͸� return,
																							//	feedback �����Ǿ� �ִ��� Ȯ���ϱ� ���� ...
	
	//// route ���� �Լ�															
	/////////////// input to DFFSR
	vector<vector<vector<Gate>>>all_input2DFF_route;
	vector<vector<Gate>>input2DFF_info;
	void input2DFFSR_route(const Gate& g, vector<Gate>gate_route);

	///////////////	Data path route �������� ���
	void route(const Gate& g, vector<Gate>gate_route);	// clock path �������� �ʴ� ��
	vector<vector<Gate>> route_info;
	vector<vector<vector<Gate>>> all_route_info;	// output ���� ���� ���� �� output���� route ����
	
	/////////////// Clock path route 
	void clk_route(const Gate& g, vector<Gate>gate_route);	// clock path�� ����
	vector<vector<vector<Gate>>> all_clk_route_info;	//  output ���� ���� ���� �� output ���� clock route ����
	vector<vector<Gate>> clk_route_info;	//	output 1�� �� �� route
	vector<int>CLK_Load;					// CLKBUF �Ʒ��� �����ϴ� load ����
	
	/////////////// Pre-route Random clock buffer insertion////////////////////////
	vector<vector<vector<Gate>>> CLKBUF_Load;				// CLKBUF �Ʒ��� Load �����ϴ� vector
	void Random_CLKBUF_insertion(vector<vector<Gate>> clk_route_Load, int Max_CLK_Load);
	vector<vector<Gate>> PreRoute_CLKBUF;					// Pre-route �������� �߻��� random buffer insertion ���� ����
	int PreRoute_CLKBUF_num = 0;
	void Pre_route_print();									// Pre-route ���� �� verilog ���� ���

	/////////////// clock to DFF path delay ���, Pre-route ���Ŀ� ����
	int DFFSR_COUNT = 0;							// clkbuf to DFFSR delay�� ����
	vector<float> clockpath_delay;	// clock to DFFSR clock port������ delay�� ��ϵ� ��
	void Note_clocktoDFF();			// ����� route���� DFF to DFF path�� ���� delay ���, max delay ����

	/////////////// Clockpath route && buffer insertion
	vector<vector<vector<Gate>>> CTS_all_clk_route_info;	//  output ���� ���� ���� �� output ���� clock route ����
	vector<vector<Gate>> Clockpath_Bufferinsertion();
	vector<Gate> Clockpath_Buffer3(int clkbuf3, Gate front_Gate, Gate rear_Gate);	// Clock Buffer insertion�� Buffer insertion�� �Ȱ��� ���
	vector<Gate> Clockpath_Buffer2(int clkbuf3, int clkbuf2, Gate front_Gate, Gate rear_Gate);	// Clock Buffer insertion�� Buffer insertion�� �Ȱ��� ���
	vector<Gate> Clockpath_Buffer1(int clkbuf3, int clkbuf2, int clkbuf1, Gate front_Gate, Gate rear_Gate);	// Clock Buffer insertion�� Buffer insertion�� �Ȱ��� ���
	int CLK_BUF_NUM = 0;	// �߰��� CLKBUF �� ����, wire �����ϱ� ����
	vector<vector<Gate>> CTS_CLKBUF;
	


	//  Buffer insertion Ȱ��
	int input2DFF_NUM_BUFFER = 0;		// Buffer insertion ���� ��, ���Ե� Buffer �� 
	vector <float>DFF_delay;					// input to DFF���� �ڿ� ���ں��� DFF delay���� ������
	void input2DFF_route_delay_sum();		// input2DFF dealy ����Լ�, �� DFF���� delay ������ ��
	void input2DFF_Bufferinsertion();
	int isD_DFF(unordered_map<string, string>front_cell, unordered_map<string, string>last_cell);
	vector<Gate> BufferX2(int bufferX2, Gate front_Gate, Gate last_Gate);
	vector<Gate> BufferX4(int bufferX2, int bufferX4, Gate front_Gate, Gate last_Gate);
	pair<string, string> buffer_getinput(unordered_map<string, string>cellpin2net);	// �� ���� buffer input ��� 
	

	// insert buffer�� ������ ���Ϳ� ����
	vector<vector<Gate>> PRE_CTS_buffers;
	vector<string>pre_buffer_inputs;



	//	ǥ�� ���� ���
	void standard_deviation(vector<float> route_delay, float max_delay);

	//	Graph Visualization
	void CTS_BEFORE_Visualization(const Gate& g, vector<Gate>gate_route);
	void PRE_CTS_Visualization(const Gate& g, int level);
	void POST_CTS_Visualization(const Gate& g, int level);

	Module module;
public:
	OpenTimerParser() = default;
	virtual ~OpenTimerParser() {}
	void print();
	//void traversal();
	
	void route();
	void route_print();
	
	void Pre_route();
	
	void CTS_route();
	void CTS_route_print();

	void PRE_CTS_PRINT();
	void PRE_CTS_FILE_PRINT();
	void Net_Delay_insertion();

};
