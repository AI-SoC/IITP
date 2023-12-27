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
	vector<string> verilog_inputs;	// verilog 파일 출력할때 사용
	vector<string> verilog_outputs;	// verilog 파일 출력할 때 사용
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
	bool nostrip_isinput(const pair<string, string>& name); //vdd,gnd 제외한 input 인지 확인
	bool isclock(const unordered_map<string, string>& name);  // clock
	bool isoutput(const unordered_map<string, string>& name);
	bool isOutput_gate(const Gate Output);	// Output gate인지 확인하기

	//vector<unordered_map<string, string>>  nostrip_getchildren(const unordered_map<string, string>& cellpin2net);
	///////////////	output이 존재하는 gate vector에 모으기
	vector<Gate> Output_Gates;
	void get_output_Module();		// output이 존재하는 gate 모음
	vector<Gate> Clock_Gates;
	vector<Gate> CLKBUF_gates;		// gate 종류가 CLKBUF인 것 모음
	void get_clock_Module();		// clock이 존재하는 gate 모음
	void get_CLKBUF();				// gate 종류가 CLKBUF인 것 모음

	vector<Gate>Input_Gates;		
	void get_input_Module();		// input이 존재하는 gate 모음

	///////////////	output에서 leaf node까지의 route 모으기
	/////////////// Gate의 주소의 이차원 벡터
	void traversal(const Gate& g, int level);

	pair<string, string> get_output_Gate(const unordered_map<string, string>& name);		// output이 존재하는 port return
	pair<string, string> get_clock_input_Gate(const unordered_map<string, string>& name);	// CLK port가 존재하는 port return

	///////////////  Gate의 celltonet이 다음 Gate의 output에 있는 것을 return한다.
	vector<Gate> getchildren_Gate(const unordered_map<string, string>& pin2net, pair<string, string> gate_output);
	///////////////  CLK포트가 존재하는 gate가 있고, CLK 포트의 존재하는 넷이 현재 CLK 포트의 output 넷과 같다면  children clock gate이다.
	vector<Gate> getchildren_CLK_Gate(const Gate& g, pair<string, string> clk_port);
	///////////////
	vector<Gate> getchildren_input2DFF_Gate(const unordered_map<string, string>& pin2net, pair<string, string> input2DFF_port);

	const vector<float> stdlibrary_180nm(string gate_name);	// std cell의 delay, area, gate input의 개수
	vector<pair<string, string>> get_input_Gate(const unordered_map<string, string>& name);	// getInputs : 1개의 gate에 존재하는 input을 pair<string, string>형태의 벡터를 return,
																							//	feedback 구성되어 있는지 확인하기 위해 ...
	
	//// route 관련 함수															
	/////////////// input to DFFSR
	vector<vector<vector<Gate>>>all_input2DFF_route;
	vector<vector<Gate>>input2DFF_info;
	void input2DFFSR_route(const Gate& g, vector<Gate>gate_route);

	///////////////	Data path route 과정에서 사용
	void route(const Gate& g, vector<Gate>gate_route);	// clock path 존재하지 않는 것
	vector<vector<Gate>> route_info;
	vector<vector<vector<Gate>>> all_route_info;	// output 여러 개일 때의 각 output별로 route 저장
	
	/////////////// Clock path route 
	void clk_route(const Gate& g, vector<Gate>gate_route);	// clock path만 존재
	vector<vector<vector<Gate>>> all_clk_route_info;	//  output 여러 개일 때의 각 output 별로 clock route 저장
	vector<vector<Gate>> clk_route_info;	//	output 1개 일 때 route
	vector<int>CLK_Load;					// CLKBUF 아래에 존재하는 load 개수
	
	/////////////// Pre-route Random clock buffer insertion////////////////////////
	vector<vector<vector<Gate>>> CLKBUF_Load;				// CLKBUF 아래에 Load 저장하는 vector
	void Random_CLKBUF_insertion(vector<vector<Gate>> clk_route_Load, int Max_CLK_Load);
	vector<vector<Gate>> PreRoute_CLKBUF;					// Pre-route 과정에서 발생한 random buffer insertion 내용 저장
	int PreRoute_CLKBUF_num = 0;
	void Pre_route_print();									// Pre-route 과정 후 verilog 파일 출력

	/////////////// clock to DFF path delay 계산, Pre-route 이후에 진행
	int DFFSR_COUNT = 0;							// clkbuf to DFFSR delay의 개수
	vector<float> clockpath_delay;	// clock to DFFSR clock port마다의 delay가 기록될 곳
	void Note_clocktoDFF();			// 저장된 route에서 DFF to DFF path에 대한 delay 계산, max delay 저장

	/////////////// Clockpath route && buffer insertion
	vector<vector<vector<Gate>>> CTS_all_clk_route_info;	//  output 여러 개일 때의 각 output 별로 clock route 저장
	vector<vector<Gate>> Clockpath_Bufferinsertion();
	vector<Gate> Clockpath_Buffer3(int clkbuf3, Gate front_Gate, Gate rear_Gate);	// Clock Buffer insertion도 Buffer insertion과 똑같은 방법
	vector<Gate> Clockpath_Buffer2(int clkbuf3, int clkbuf2, Gate front_Gate, Gate rear_Gate);	// Clock Buffer insertion도 Buffer insertion과 똑같은 방법
	vector<Gate> Clockpath_Buffer1(int clkbuf3, int clkbuf2, int clkbuf1, Gate front_Gate, Gate rear_Gate);	// Clock Buffer insertion도 Buffer insertion과 똑같은 방법
	int CLK_BUF_NUM = 0;	// 추가된 CLKBUF 총 개수, wire 연결하기 위해
	vector<vector<Gate>> CTS_CLKBUF;
	


	//  Buffer insertion 활용
	int input2DFF_NUM_BUFFER = 0;		// Buffer insertion 진행 시, 삽입된 Buffer 수 
	vector <float>DFF_delay;					// input to DFF에서 뒤에 숫자보고 DFF delay별로 모으기
	void input2DFF_route_delay_sum();		// input2DFF dealy 기록함수, 각 DFF마다 delay 저장할 것
	void input2DFF_Bufferinsertion();
	int isD_DFF(unordered_map<string, string>front_cell, unordered_map<string, string>last_cell);
	vector<Gate> BufferX2(int bufferX2, Gate front_Gate, Gate last_Gate);
	vector<Gate> BufferX4(int bufferX2, int bufferX4, Gate front_Gate, Gate last_Gate);
	pair<string, string> buffer_getinput(unordered_map<string, string>cellpin2net);	// 앞 단의 buffer input 얻기 
	

	// insert buffer를 이차원 벡터에 저장
	vector<vector<Gate>> PRE_CTS_buffers;
	vector<string>pre_buffer_inputs;



	//	표준 편차 계산
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
