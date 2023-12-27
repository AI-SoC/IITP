#include "ot_parser.h"
#include <iostream>

// Struct: Gate


// A verilog parser compatible with OpenTimer
void OpenTimerParser::add_module(std::string&& name)
{
	module.name = std::move(name);
}


void OpenTimerParser::add_port(verilog::Port&& port) {
	if (port.dir == verilog::PortDirection::INPUT) {
		if (port.beg == -1)
		{
			module.inputs.push_back(port.names[0]);
			module.verilog_inputs.push_back(port.names[0]);		//verilog ����� �� input ����
		}
		else
		{
			for (size_t i = 0; i <= port.beg; i++)
			{
				string tmp = "[" + to_string(i) + "]";
				module.inputs.push_back(port.names[0] + (tmp));
			}
			string verilog = "[" + to_string(port.beg) + ":0]";
			module.verilog_inputs.push_back(verilog + port.names[0]);		//verilog ����� �� input ����
		}
	}
	else if (port.dir == verilog::PortDirection::OUTPUT) {
		if (port.beg == -1)
		{
			module.outputs.push_back(port.names[0]);
			module.verilog_outputs.push_back(port.names[0]);		//verilog ����� �� input ����
		}
		else
		{
			for (size_t i = 0; i <= port.beg; i++)
			{
				string tmp = "[" + to_string(i) + "]";
				module.outputs.push_back(port.names[0] + (tmp));
			}
			string verilog = "[" + to_string(port.beg) + ":0]";
			module.verilog_outputs.push_back(verilog + port.names[0]);		//verilog ����� �� input ����
		}
		//module.outputs.push_back(port.names[0]);
	}
	else {
		module.inputs.insert(module.inputs.end(), port.names.begin(), port.names.end());
		module.outputs.insert(module.outputs.end(), port.names.begin(), port.names.end());
	}
	std::move(port.names.begin(), port.names.end(), std::back_inserter(module.ports));
	std::cout << "port : " << port << std::endl;
}


void OpenTimerParser::add_net(verilog::Net&& net) {
	if (net.type == verilog::NetType::WIRE) {
		std::move(net.names.begin(), net.names.end(), std::back_inserter(module.wires));
	}
}


void OpenTimerParser::add_assignment(verilog::Assignment&& ast) {
	// Skip assignment
}


void OpenTimerParser::add_instance(verilog::Instance&& inst) {
	auto& g = module.gates.emplace_back();
	g.name = inst.module_name;
	g.cell = inst.inst_name;
	std::cout << std::endl;
	std::cout << "g.name : " << g.name << " g.cell : " << g.cell << std::endl;
	fflush(stdin);
	std::string pin;
	std::string net;
	for (size_t i = 0; i < inst.net_names.size(); i++) {
		auto& net_name = inst.net_names[i][0];
		if (i < inst.net_names.size()) {
			auto& pin_name = inst.pin_names[i];
			switch (pin_name.index()) {
			case 0: pin = std::get<0>(pin_name); break;
			case 1: pin = std::get<1>(pin_name).name; break;
			case 2: pin = std::get<2>(pin_name).name; break;
			}

			switch (net_name.index()) {
			case 0: net = std::get<0>(net_name); break;
			case 1: net = std::get<1>(net_name).name + '[' + std::get<1>(net_name).bbit + ']'; break;
			case 2: net = std::get<2>(net_name).name; break;
			case 3: net = std::get<3>(net_name).value; break;
			}

			g.cellpin2net.insert({ pin, net });
			std::cout << "g.cellpin2net : " << pin << " " << g.cellpin2net[pin] << " " << std::endl;
			g.net2cellpin.insert({ std::move(net), std::move(pin) });
			//std::cout << "g.net2cellpin : " << net << g.net2cellpin[pin] << std::endl;
			std::cout << std::endl;
		}
	}
}



void OpenTimerParser::print()
{
	std::cout << "module.name : " << module.name << std::endl;
	for (auto i : module.inputs)
		std::cout << "ot.module.inputs : " << i << std::endl;
	
	for (auto o : module.outputs)
		std::cout << "ot.module.outputs : " << o << std::endl;

	for (size_t i = 0; i < module.gates.size(); i++)
	{
		std::cout << "name : " << module.gates[i].name << std::endl;
		std::cout << "cell : " << module.gates[i].cell << std::endl;
		fflush(stdin);
		for (auto elem : module.gates[i].cellpin2net)
			std::cout << "key : " << elem.first << " value : " << elem.second << std::endl;
		fflush(stdin);
		/*
		for (auto elem : module.gates[i].cellpin2net)
			if (nostrip_isinput(elem))
				module.gates[i].gate_input += 1;

		std::cout << "gate_input : " << module.gates[i].gate_input << std::endl;
		*/
	}
}




// input���� Ȯ��
bool OpenTimerParser::isinput(const pair<string, string>& name)
{
	for (size_t i = 0; i < module.inputs.size(); i++)
		if (name.second == module.inputs[i])
			return true;
	return false;
}


// power strip & clock port  �����ϰ� input �Ǵ��ϱ�
bool OpenTimerParser::nostrip_isinput(const pair<string, string>& name)
{
	for (size_t i = 0; i < module.inputs.size(); i++)
		if (name.second == module.inputs[i] && name.first != "CLK" && name.first != "vdd" && name.first != "gnd")
			return true;
	return false;
}


// clock���� Ȯ��
bool OpenTimerParser::isclock(const unordered_map<string, string>& name)
{
	for (auto elem : name)
		if (elem.second == "clock" || elem.second == "clk")
			return true;
	return false;
}


// output���� Ȯ��
bool OpenTimerParser::isoutput(const unordered_map<string, string>& name)
{
	for (auto o : module.outputs)
	{
		for (auto elem : name)
			if (elem.second == o)
				return true;
	}
	return false;
}


//	output port�� �����ϴ� gate���� Ȯ��
bool OpenTimerParser::isOutput_gate(const Gate Output)
{
	for (auto o : Output_Gates)
	{
		if (Output.cell == o.cell)
			return true;
	}
	return false;
}


//	output�� �����ϴ� gate ����, ���� ���� output�� ������ �� output�� ����ִ� ����
void OpenTimerParser::get_output_Module()
{
	for (size_t i = 0; i < module.gates.size(); i++)
	{
		if (isoutput(module.gates[i].cellpin2net))
			Output_Gates.push_back(module.gates[i]);
	}
	cout << "===============================Output Gate=================================" << endl;
	for (auto a : Output_Gates)
		cout << "cell name : " << a.cell << endl;
	return;
}

//	clk port�� �����ϴ� gate ����
void OpenTimerParser::get_clock_Module()
{
	for (size_t i = 0; i < module.gates.size(); i++)
	{
		if (isclock(module.gates[i].cellpin2net))
			Clock_Gates.push_back(module.gates[i]);
	}
	cout << "===============================Clock Gate=================================" << endl;
	for (auto a : Clock_Gates)
		cout << "cell name : " << a.cell << endl;
	return;
}

// gate ������ CLKBUF�� �� ����
void OpenTimerParser::get_CLKBUF()
{
	for (size_t i = 0; i < module.gates.size(); i++)
	{
		if (module.gates[i].cell.find("CLKBUF")!=string::npos)
			CLKBUF_gates.push_back(module.gates[i]);
	}
	cout << "===============================CLKBUF_gates=================================" << endl;
	for (auto a : CLKBUF_gates)
		cout << "cell name : " << a.cell << endl;
	return;
}

//	input port�� �����ϴ� gate ���� (input clock�� ��� �ִ� gate ����)
void OpenTimerParser::get_input_Module()
{
	for (size_t i = 0; i < module.gates.size(); i++)
	{
		for(auto a : module.gates[i].cellpin2net)
		if (nostrip_isinput(a) && a.second != "clock" && a.second != "clk")
		{
			Input_Gates.push_back(module.gates[i]);
			break;
		}
	}
	cout << "===============================Input Gate=================================" << endl;
	for (auto a : Input_Gates)
		cout << "cell name : " << a.cell << endl;
	return;
}


//	get_output_Gate : ���� gate�� output�� ã�� ���� ��
pair<string, string> OpenTimerParser::get_output_Gate(const unordered_map<string, string>& name)
{
	for (auto a : name)
		if (a.first == "Y" || a.first == "Q")
			return a;
}


//	getInputs : gate �ܿ� �����ϴ� input port net ����, output��Ʈ�� clk��Ʈ�� �����ϰ� �� ��
vector<pair<string, string>> OpenTimerParser::get_input_Gate(const unordered_map<string, string>& name)
{
	vector<pair<string, string>> g_inputs;
	for (auto a : name)
	{
		if (a.first != "Y" && a.first != "Q" && a.first!="CLK")
			g_inputs.push_back(a);
	}
	return g_inputs;
}


//	CLK port �κ��� net ����
pair<string, string> OpenTimerParser::get_clock_input_Gate(const unordered_map<string, string>& name)
{
	for (auto a : name)
		if (a.first == "CLK")
			return a;
}

/*
// traversal ����
void OpenTimerParser::traversal()
{
	get_output_Module();
	
	for (int i = 0; i < Output_Gates.size(); i++)
		traversal(Output_Gates[i], 0);
}

// traversal ����
void OpenTimerParser::traversal(const Gate& g, int level)
{
	for (int i = 0; i < level; i++)
		cout << "--" << " ";
	cout << "g.name : " << g.name << endl;

	for (auto net : g.cellpin2net)
	{
		if (nostrip_isinput(net))
			return;
		else
			break;
	}
	pair<string, string> gate_output = get_output_Gate(g.cellpin2net);
	vector<Gate>children_Gate = getchildren_Gate(g.cellpin2net, gate_output);
	for (int i = 0; i < children_Gate.size(); i++)
		traversal(children_Gate[i], level + 1);
	return;
}
*/


//	data path Ž���� �� children ���� ��� output-gate-input <---> output-gate-input �̷��� ����Ǿ�� ��
vector<Gate> OpenTimerParser::getchildren_Gate(const unordered_map<string, string>& pin2net, pair<string, string> gate_output)
{
	vector<Gate> get_children;
	for (int i = 0; i < module.gates.size(); i++)
		for (auto n : module.gates[i].cellpin2net)
			if (n.first == "Q" || n.first == "Y")
				for (auto elem : pin2net)
					if (elem.first != "Q" && elem.first != "Y" && elem.first != "vdd" && elem.first != "gnd" && elem.first != "CLK"&& module.gates[i].name.find("CLKBUF") == string::npos)
						if (elem.second == n.second)
							get_children.push_back(module.gates[i]);
	// get_children ���� Ȯ���ϱ�
	/*
	cout << " get_children ���� Ȯ���ϱ�" << endl;
	for (auto a : get_children)
		cout << a.cell << " ";
	cout << endl;
	*/
	return get_children;
}

//	CLK path Ž���� �� children ���� ��� input - gate - clk <---> clk - gate - output �̷��� ����Ǿ�� ��
vector<Gate> OpenTimerParser::getchildren_CLK_Gate(const Gate& g, pair<string, string> clk_port)
{
	vector<Gate> get_clk_children;
	for (int i = 0; i < module.gates.size(); i++)
		for (auto n : module.gates[i].cellpin2net)
		{
			if(g.cell != module.gates[i].cell)
				if (n.first == "A" || n.first == "CLK")
				{
					if (clk_port.second == n.second && (n.second.find("clk") != string::npos || n.second.find("clock") != string::npos))
						get_clk_children.push_back(module.gates[i]);
				}
		}
			
	// get_clk_children ���� Ȯ���ϱ�
	/*
	for (auto a : get_clk_children)
		cout << a.cell << " ";
	cout << endl;
	*/
	return get_clk_children;
}

//	input2DFFpath Ž���� �� children ���� ��� input-gate-output <---> input-gate-output �̷��� ����Ǿ�� ��
vector<Gate> OpenTimerParser::getchildren_input2DFF_Gate(const unordered_map<string, string>& pin2net, pair<string, string> gate_output)
{
	vector<Gate> get_children;
	for (int i = 0; i < module.gates.size(); i++)
		for (auto n : module.gates[i].cellpin2net)
			if (n.first != "Q" && n.first != "Y" && n.first != "vdd" && n.first != "gnd" \
				&& n.first != "CLK" && module.gates[i].name.find("CLKBUF") == string::npos && n.first!="S"&&n.first!="R")
				if (gate_output.second == n.second)
					get_children.push_back(module.gates[i]);
	// get_children ���� Ȯ���ϱ�
	/*
	for (auto a : get_children)
		cout << a.cell << " ";
	cout << endl;
	*/
	return get_children;
}

//	datapath, clkpath Ž���Ͽ� 1���� output�� ������ route�� ����
void OpenTimerParser::route()
{
	get_output_Module();	// output�� ���ԵǾ� �ִ� gate ����
	get_clock_Module();		// clk�� ���ԵǾ� �ִ�  gate ����
	get_input_Module();		// input(clock ����) ���ԵǾ� �ִ� gate ����
	get_CLKBUF();			// CLKBUF gate ����
	
	//	clkpath Ž�� CLKtoDFFSR
	vector <Gate> clk_gate_route;
	for (size_t i = 0; i < Clock_Gates.size(); i++)
	{
		clk_route_info.clear();
		cout << "======================== Clock route " << Clock_Gates[i].cell << "========================" << endl;
		fflush(stdin);
		clk_route(Clock_Gates[i], clk_gate_route);
		all_clk_route_info.push_back(clk_route_info);
	}
	
	return;
}



// clkpath route ���� ����
void OpenTimerParser::clk_route(const Gate& g, vector<Gate>gate_route)
{
	//	������ cell name�� ����صξ��� cell name�� ��ġ�Ѵٸ� ����
	cout << "clk gate : " << g.cell << endl;
	gate_route.push_back(g);
	pair<string, string> gate_clock_output = get_output_Gate(g.cellpin2net);
	//cout << "gate_clock_output : " << gate_clock_output.first << " " << gate_clock_output.second << endl;
	
	vector<Gate>children_CLK_Gate = getchildren_CLK_Gate(g, gate_clock_output);
	if (children_CLK_Gate.size() == 0)
	{
		DFFSR_COUNT+=1;
		clk_route_info.push_back(gate_route);
		return;
	}

	for (int i = 0; i < children_CLK_Gate.size(); i++)
	{
		clk_route(children_CLK_Gate[i], gate_route);
	}
	return;
}

// datapath route ���� ����
void OpenTimerParser::route(const Gate& g, vector<Gate>gate_route)
{
	//	������ cell name�� ����صξ��� cell name�� ��ġ�Ѵٸ� ����
	cout << "current gate : " << g.cell << endl;
	for (size_t a = 0; a < gate_route.size(); a++)
	{
		if (gate_route[a].cell == g.cell)
		{
			gate_route.push_back(g);
			cout << "==================================" << endl;
			route_info.push_back(gate_route);
			return;
			/*
			pair<string, string> a_next_output = get_output_Gate(gate_route[a + 1].cellpin2net);		// route ���� ���� �����ϴ� inputs
			vector<pair<string, string>> g_inputs = get_input_Gate(g.cellpin2net);	// ���� ����Ʈ�� �����ϴ� inputs
			for (auto i : g_inputs)
				if (a_next_output.second == i.second)
				{
					gate_route.push_back(g);
					route_info.push_back(gate_route);
					return;
				}
				*/
		}
	}

	gate_route.push_back(g);

	float input_cnt = 0;	// ���� gate���� ������ �ִ� module input�� ����
	for (auto net : g.cellpin2net)
		if (nostrip_isinput(net))
			input_cnt++;
	vector<float> tmp_gate = stdlibrary_180nm(g.name);
	// cout << "tmp_gate : " << g.name << " " << tmp_gate[2] << endl;
	//	�������� : gate_input������ module input ���� ��ġ�Ѵٸ� return

	if (input_cnt == tmp_gate[2])
	{
		route_info.push_back(gate_route);
		cout << "==============================route end=======================================" << endl;
		return;
	}
	else if (input_cnt != 0 && input_cnt != tmp_gate[2])
	{
		route_info.push_back(gate_route);
	}

	pair<string, string> gate_output = get_output_Gate(g.cellpin2net);
	vector<Gate>children_Gate = getchildren_Gate(g.cellpin2net, gate_output);

	for (int i = 0; i < children_Gate.size(); i++)
		route(children_Gate[i], gate_route);
	return;
}



// input2DFF path ���ϱ�, input(clock ����)���� DFF�� route ����
void OpenTimerParser::input2DFFSR_route(const Gate& g, vector<Gate>gate_route)
{
	//	������ cell name==DFFSR�̶�� ����
	cout << "input2DFFSR gate : " << g.cell << endl;
	gate_route.push_back(g);
	pair<string, string> gate_in2DFF_output = get_output_Gate(g.cellpin2net);
	vector<Gate>children_in2DFF_Gate = getchildren_input2DFF_Gate(g.cellpin2net, gate_in2DFF_output);
	if (g.name == "DFFSR" || g.name == "DFFPOSX1")
	{
		input2DFF_info.push_back(gate_route);
		return;
	}

	for (int i = 0; i < children_in2DFF_Gate.size(); i++)
	{
		input2DFFSR_route(children_in2DFF_Gate[i], gate_route);
	}
	return;
}



//	route Ȯ���ϱ� ���� print�ϴ� �Լ�
void OpenTimerParser::route_print()
{
	// ������ verilog ���� ���� ����
	//////////////////////// Output Gate���� �з��ϱ�////////////////////////////////////////
	CLK_Load.resize(CLKBUF_gates.size());
	//cout << "CLKBUF_gates.size() : " << CLKBUF_gates.size() << endl;
	
	cout << "////////////////////////////////////////////////////// Output Clock Gate���� �з��ϱ�/////////////////////////////////////////////////////////////" << endl;
	for (size_t i = 0; i < all_clk_route_info.size(); i++)
	{
		cout << "================================== Clock Path ===================================" << endl;
		cout << "clock�� �����ϴ� input gate�� ���� route ���� : " << all_clk_route_info[i].size() << endl;
		
		int CLKBUF_index = 0;
		for (size_t j = 0; j < all_clk_route_info[i].size(); j++)
		{
			vector<Gate>temp_Load;
			for (size_t k = 0; k < all_clk_route_info[i][j].size(); k++)
			{
				if (all_clk_route_info[i][j][k].cell.find("CLKBUF") != string::npos)
				{
					CLKBUF_index = stoi(all_clk_route_info[i][j][k].cell.substr(8)) - 1;
					CLK_Load[CLKBUF_index]++;		// CLKBUF���� �޷��ִ� load ���
					
				}
				cout <<"i j k : " << i << j << k << " : " << all_clk_route_info[i][j][k].cell << " --> ";
				fflush(stdin);
			}
			cout << endl;
		}
	}


	//cout << "==================================== Pre-route random buffer insertion ================================================" << endl;
	
	//cout << "==================================== clock to DFF path �� delay ���� ū �� ===============================================" << endl;
	//Note_clocktoDFF();
	//cout << "==================================== Clock tree synthesis ================================================" << endl;
	//Clockpath_Bufferinsertion();
}

/////// ��ձ��ϴ� template
template<typename T>
double getAverage(std::vector<T> const& v) {
	if (v.empty()) {
		return 0;
	}
	return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
}


void OpenTimerParser::Pre_route()
{
	// Load�� ��� ���ϱ�
	double Load_mean = getAverage(CLK_Load);
	
	int maxLoad_index= max_element(CLK_Load.begin(), CLK_Load.end()) - CLK_Load.begin();
	cout << "Load mean : " << Load_mean << "  maxLoad_index : "<<maxLoad_index<< endl;

	// ���� CLKBUF���� ���� 2���� ���Ϳ� ���� random buffer insertion ����
	int temp_CLKBUF_index = 0;
	vector<vector<Gate>> temp_Gates2;
	vector<Gate>temp_Gates1;

	for (int a = 0; a < all_clk_route_info.size(); a++)
	{
		int CLKBUF_index = 0;	
		for (int b = 0; b < all_clk_route_info[a].size(); b++)
		{
			for (int c = 0; c < all_clk_route_info[a][b].size(); c++)
			{
				if (all_clk_route_info[a][b][c].cell.find("CLKBUF") != string::npos || all_clk_route_info[a][b][c].cell.find("DFF") != string::npos)
				{
					temp_Gates1.push_back(all_clk_route_info[a][b][c]);
					temp_CLKBUF_index = CLKBUF_index;
					if (all_clk_route_info[a][b][c].cell.find("CLKBUF") != string::npos)
						CLKBUF_index = stoi(all_clk_route_info[a][b][c].cell.substr(8)) - 1;
				}

				if (all_clk_route_info[a][b][c].cell.find("DFF") != string::npos)
				{
					temp_Gates2.push_back(temp_Gates1);
					temp_Gates1.clear();
				}

				if (temp_CLKBUF_index != CLKBUF_index)
				{
					CLKBUF_Load.push_back(temp_Gates2);
					temp_Gates2.clear();
				}

			}
		}
	}
	

	for (int i = 0; i < CLKBUF_Load.size(); i++)
	{
		cout << "====================================================" << endl;
		for (int j = 0; j < CLKBUF_Load[i].size(); j++)
		{
			for (int k = 0; k < CLKBUF_Load[i][j].size(); k++)
				cout << CLKBUF_Load[i][j][k].cell << " => ";

			cout << endl <<"CLKBUF_Load[i].size() : "<< CLKBUF_Load[i].size()<< endl;

			if (CLKBUF_Load[i].size() < CLK_Load[maxLoad_index])
				Random_CLKBUF_insertion(CLKBUF_Load[i], CLK_Load[maxLoad_index]); 
		}
	}

					
	Pre_route_print();
}


void OpenTimerParser::Random_CLKBUF_insertion(vector<vector<Gate>> clk_route_Load, int Max_CLK_Load)
{
	////// clk_route_Load ���� ���
	/*
	for (size_t i = 0; i < clk_route_Load.size(); i++)
	{
		for (size_t j = 0; j < clk_route_Load[i].size(); j++)
			cout << clk_route_Load[i][j].cell << "  " << endl;
	}*/

	int CLK_Load = clk_route_Load.size();

	cout << "CLK load : " << CLK_Load << "  Max_CLK_Load : " << Max_CLK_Load << endl;
	if (CLK_Load <Max_CLK_Load)
	{
		if (CLK_Load <= 2)
		{
			// instance���� strength�� �ٲٱ�
			for (size_t a = 0; a < module.gates.size(); a++)
			{
				if (stoi(module.gates[a].cell.substr(8)) == stoi(clk_route_Load[0][0].cell.substr(8)))
				{
					module.gates[a].name = "CLKBUF2";
					module.gates[a].cell = "CLKBUF2__a";
				}
			}
		}

		vector<Gate> random_buffers;			// ������ clock path���� random_buffers ���� ����
		vector<Gate> Original_DFFS;				// DFF�� clk port�� �ٲٱ� ���� clock path�� �ش��ϴ� DFF�� ������
		Gate clkbuf;
		clkbuf.name = "";
		clkbuf.cell = "";

		// ���� CLKBUF�� output�� �̱�
		pair<string, string> Original_clkbuf_output = get_output_Gate(clk_route_Load[0][0].cellpin2net);		// �մ��� output ���� ��������

		// ���� clock path�� �����ϴ� DFF ������
		for (int i = 0; i < clk_route_Load.size(); i++)
		{
			for (int j = 0; j < clk_route_Load[i].size(); j++)
				if (clk_route_Load[i][j].cell.find("DFF") != string::npos)
					Original_DFFS.push_back(clk_route_Load[i][j]);
		}

		// Random buffer insertion ����
		if (CLK_Load % 2 == 0 && CLK_Load > 2)
		{	// CLK_Load�� ¦���϶�

			for (int i = 0; i < 2; i++)
			{
				clkbuf.name = "CLKBUF3";
				clkbuf.cell = "CLKBUF3__" + to_string(++PreRoute_CLKBUF_num);
				pair<string, string> clkbuf_input = { "A", Original_clkbuf_output.second };			//	CLKBUF Y port�� ���� == ���Ե� CLKBUF A port ����
				pair<string, string> clkbuf_output = { "Y","random_clk_bf3__" + to_string(PreRoute_CLKBUF_num) };	//	���Ե� CLKBUF Y port ���� = ���� �����
				clkbuf.cellpin2net["A"] = clkbuf_input.second;
				clkbuf.cellpin2net["Y"] = clkbuf_output.second;

				int flag = false;
				for (size_t i = 0; i < module.gates.size(); i++)
				{
					for (auto a : module.gates[i].cellpin2net)
					{
						if (a.second == Original_clkbuf_output.second && module.gates[i].cell.find("DFF") != string::npos)
						{
							if (module.gates[i].cellpin2net["CLK"].find("random") == string::npos)
							{
								flag = true;
							}
							else
								break;
						}
					}
				}

				if(flag==true)
					random_buffers.push_back(clkbuf);

				int change_cnt = 0;		// CLK_Load/2�� �ٲ�� �ϱ� ������..
				
				// ���� module instance D flip flop CLK port ����
				for (size_t i = 0; i < module.gates.size(); i++)
				{
					for (auto a : module.gates[i].cellpin2net)
					{
						if (a.second == Original_clkbuf_output.second && module.gates[i].cell.find("DFF")!=string::npos)
						{
							if (module.gates[i].cellpin2net["CLK"].find("random") == string::npos)
							{
								module.gates[i].cellpin2net["CLK"] = clkbuf_output.second;
								cout << "module.gates[i].cellpin2net : " << module.gates[i].cellpin2net["CLK"] << endl;
								change_cnt++;
							}
							else
								break;
						}
					}
					if (change_cnt == CLK_Load/2)
					{
						change_cnt = 0;
						break;
					}
				}
			}


			
			/*
			int load_cnt = 0;
			for (size_t j = 0; j < module.gates.size(); j++)
			{
				for (size_t a = 0; a < Original_DFFS.size(); a++)
				{
					if (module.gates[j].cell == Original_DFFS[a].cell)
					{
						module.gates[j].cellpin2net["CLK"] = random_buffers[0].cellpin2net["Y"];	// CLKBUF output �κ��� DFF clock port�� ����
						load_cnt++;
						if (CLK_Load / 2 < load_cnt)
						{
							module.gates[j].cellpin2net["CLK"] = random_buffers[1].cellpin2net["Y"];	// CLKBUF output �κ��� DFF clock port�� ����
							load_cnt++;
						}
					}
				}
			}*/
		}
		else
		{
			int CLK_Load1 = CLK_Load / 2;
			int CLK_Load2 = CLK_Load - CLK_Load1;
		}

		PreRoute_CLKBUF.push_back(random_buffers);
		cout << "PreRoute_CLKBUF : " << PreRoute_CLKBUF.size() << " random buffer insertion num : " << PreRoute_CLKBUF_num << endl;
	}
	else
		return;
}

void OpenTimerParser::Pre_route_print()
{
	cout << "=============================== Pre-route proccess after random buffer insertion ===============================" << endl;
	cout << "module " << module.name << "(" << endl;
	for (auto a : module.verilog_inputs)
		cout << "  input " << a << "," << endl;
	for (int i = 0; i < module.verilog_outputs.size() - 1; i++)
		cout << "  output " << module.verilog_outputs[i] << "," << endl;
	cout << "  output " << module.verilog_outputs[module.verilog_outputs.size() - 1] << endl;
	cout << ");" << endl;

	for (auto a : module.gates)
	{
		cout << "  " << a.name << " " << a.cell << " (";
		for (auto b : a.cellpin2net)
			if (b.first != "gnd" && b.first != "vdd")
				cout << "." << b.first << "(" << b.second << "), ";
		cout << "\b\b);" << endl;
	}

	for (int i = 0; i < PreRoute_CLKBUF.size(); i++)
	{
		for (int j = 0; j < PreRoute_CLKBUF[i].size(); j++)
		{
			// ���� verilog instance �߰�
			module.gates.push_back(PreRoute_CLKBUF[i][j]);
			cout << "  " << PreRoute_CLKBUF[i][j].name << " " << PreRoute_CLKBUF[i][j].cell << " (";
			for (auto b : PreRoute_CLKBUF[i][j].cellpin2net)
				cout << "." << b.first << "(" << b.second << "), ";
			cout << "\b\b);" << endl;
		}
	}

	cout << "endmodule" << endl;
	cout << endl;
}

void OpenTimerParser::CTS_route()
{
	get_output_Module();	// output�� ���ԵǾ� �ִ� gate ����
	//get_clock_Module();		// clk�� ���ԵǾ� �ִ�  gate ����
	get_input_Module();		// input(clock ����) ���ԵǾ� �ִ� gate ����
	get_CLKBUF();			// CLKBUF gate ����
	
	DFFSR_COUNT = 0;

	//	clkpath Ž�� CLKtoDFFSR
	vector <Gate> clk_gate_route;
	for (size_t i = 0; i < Clock_Gates.size(); i++)
	{
		clk_route_info.clear();
		cout << "======================== Clock route " << Clock_Gates[i].cell << "========================" << endl;
		fflush(stdin);
		clk_route(Clock_Gates[i], clk_gate_route);
		CTS_all_clk_route_info.push_back(clk_route_info);
	}
	return;
}

//	route Ȯ���ϱ� ���� print�ϴ� �Լ�
void OpenTimerParser::CTS_route_print()
{
	cout << "CTS_all_clk_route_info.size()  : " << CTS_all_clk_route_info.size() << endl;
	cout << "all_clk_route_info.size()  : " << all_clk_route_info.size() << endl;
	cout << "////////////////////////////////////////////////////// Output Clock Gate���� �з��ϱ�/////////////////////////////////////////////////////////////" << endl;
	for (size_t i = 0; i < all_clk_route_info.size(); i++)
	{
		cout << "================================== Clock Path ===================================" << endl;
		cout << "clock�� �����ϴ� input gate�� ���� route ���� : " << all_clk_route_info[i].size() << endl;
		for (size_t j = 0; j < CTS_all_clk_route_info[i].size(); j++)
		{
			int CLKBUF_index = 0;
			for (size_t k = 0; k < CTS_all_clk_route_info[i][j].size(); k++)
			{
				cout << CTS_all_clk_route_info[i][j][k].cell << " --> ";
				fflush(stdin);
			}
			cout << endl;
		}
	}


	//cout << "==================================== Pre-route random buffer insertion ================================================" << endl;
	//cout << "==================================== clock to DFF path �� delay ���� ū �� ===============================================" << endl;
	Note_clocktoDFF();
	//cout << "==================================== Clock tree synthesis ================================================" << endl;
	Clockpath_Bufferinsertion();
}


void OpenTimerParser::Note_clocktoDFF()
{
	clockpath_delay.resize(DFFSR_COUNT);
	cout << "DFFSR count : " << DFFSR_COUNT << endl;

	for (size_t i = 0; i < CTS_all_clk_route_info.size(); i++)
	{
		for (size_t j = 0; j < CTS_all_clk_route_info[i].size(); j++)
		{
			float delay = 0;			// 1���� route�� DFFSR to DFFSR path delay 
			for (size_t k = 0; k < CTS_all_clk_route_info[i][j].size(); k++)
			{
				//	input �ִ� ������ Ž���ϱ�.
				vector<float> inst_delay = stdlibrary_180nm(CTS_all_clk_route_info[i][j][k].name);	// gate�� delay ��������

				// route�� ������ �κ��� DFF�� �������� ���� �� delay ������� ����
				int cnt = 0;
				delay += inst_delay[0];
				cnt++;
				if (CTS_all_clk_route_info[i][j][k].name == "DFFSR")
				{
					string st = CTS_all_clk_route_info[i][j][k].cell.substr(6);	// DFFSR �޺κ��� ���� ����
					int dffsr_num = stoi(st) - 1;
					if (clockpath_delay[dffsr_num] == 0)
					{
						// ���� �ε��� �ڸ��� delay ���� ���ٸ�? �׳� ����
						clockpath_delay[dffsr_num] = delay;
					}
					else
					{
						// ���� �ε��� �ڸ� ���� ���� delay �� ��
						if (clockpath_delay[dffsr_num] < delay)
							clockpath_delay[dffsr_num] = delay;
					}
					delay = 0;
				}
				else if (CTS_all_clk_route_info[i][j][k].name == "DFFPOSX1" || CTS_all_clk_route_info[i][j][k].name == "DFFNEGX1")
				{
					string st = CTS_all_clk_route_info[i][j][k].cell.substr(9);	// DFFSR �޺κ��� ���� ����
					int dffsr_num = stoi(st) - 1;
					if (clockpath_delay[dffsr_num] == 0)
					{
						// ���� �ε��� �ڸ��� delay ���� ���ٸ�? �׳� ����
						clockpath_delay[dffsr_num] = delay;
					}
					else
					{
						// ���� �ε��� �ڸ� ���� ���� delay �� ��
						if (clockpath_delay[dffsr_num] < delay)
							clockpath_delay[dffsr_num] = delay;
					}
					delay = 0;
				}
			}
		}
	}

	cout << "======================================== clockpath_delay check ================================================" << endl;
	cout << "[" << endl;
	for (int i=0; i<clockpath_delay.size(); i++)
	{
		cout  <<clockpath_delay[i] <<"," << endl;
		fflush(stdin);
	}cout << "]" << endl;
	cout << "clockpath_delay.size() : " << clockpath_delay.size() << endl;
}



vector<vector<Gate>>OpenTimerParser::Clockpath_Bufferinsertion()
{
	cout << "////////////////////////////////////////// Clock Buffer insertion //////////////////////////////////////////////////////////////" << endl;
	float max_clockpath_delay = *max_element(clockpath_delay.begin(), clockpath_delay.end());		// DFF to DFF�� ���� max delay
	int max_clockpath_index = max_element(clockpath_delay.begin(), clockpath_delay.end()) - clockpath_delay.begin();	// DFF to DFF�� ���� max delay�� index

	vector<vector<Gate>> insert_clkbuffers;		// input2DFF path���� buffer insertion

	for (size_t i = 0; i < CTS_all_clk_route_info.size(); i++)
	{
		for (size_t j = 0; j < CTS_all_clk_route_info[i].size(); j++)
		{
			int index;
			int size = CTS_all_clk_route_info[i][j].size() - 1;	// CTS_all_clk_route_info[i][j][1]�� ����
			if (CTS_all_clk_route_info[i][j][size].name == "DFFSR")
				index = stoi(CTS_all_clk_route_info[i][j][size].cell.substr(6)) - 1;	// DFFSR_maxdelay�� �ش��ϴ� index == DFFSR ���ڿ��� 1�� ����'
			else if (CTS_all_clk_route_info[i][j][size].name == "DFFPOSX1" || CTS_all_clk_route_info[i][j][size].name == "DFFNEGX1")
				index = stoi(CTS_all_clk_route_info[i][j][size].cell.substr(9)) - 1;	// DFFSR_maxdelay�� �ش��ϴ� index == DFFSR ���ڿ��� 1�� ����'

			// ���� max delay index�� ���� �ʴٸ�, buffer ���� �ؾ���.
			if (max_clockpath_index != index)
			{
				float clock_skew = max_clockpath_delay - clockpath_delay[index];	// max delay�� ���� delay ����
				// CLKBUF3 insertion
				cout << endl << "index : " << index << " DFFSR_maxdelay : " << clockpath_delay[index] << endl;
				fflush(stdin);
				int CLK_BUF3 = clock_skew / 0.079;	// CLK_BUF3�� ���� 

				if (CLK_BUF3 < 0)
					CLK_BUF3 = 0;
				else
					clock_skew = clock_skew - CLK_BUF3 * 0.079;	// clock skew ����

				cout << "CLK_BUF3 : " << CLK_BUF3 << endl;
				fflush(stdin);
				CLK_BUF_NUM += CLK_BUF3;

				vector<Gate>insert_clkbuf3;
				if (CLK_BUF3 > 0)
				{
					insert_clkbuf3 = Clockpath_Buffer3(CLK_BUF3, CTS_all_clk_route_info[i][j][size - 1], CTS_all_clk_route_info[i][j][size]);
					clockpath_delay[index] += CLK_BUF3 * 0.079;
				}
				//	clock route���� buffer insertion ����, input�� ���� ��� ������ �� �����Ƿ�, delay ����
				for (int a = 0; a < insert_clkbuf3.size(); a++)
					CTS_all_clk_route_info[i][j].push_back(insert_clkbuf3[a]);


				// CLKBUF2 insertion
				cout << endl << "index : " << index << " DFFSR_maxdelay : " << clockpath_delay[index] << endl;
				fflush(stdin);
				int CLK_BUF2 = clock_skew / 0.072;	// CLK_BUF2�� ���� 

				if (CLK_BUF2 < 0)
					CLK_BUF2 = 0;
				else
					clock_skew = clock_skew - CLK_BUF2 * 0.072;	// clock skew ����

				cout << "CLK_BUF2 : " << CLK_BUF2 << endl;
				fflush(stdin);
				CLK_BUF_NUM += CLK_BUF2;

				vector<Gate>insert_clkbuf2;
				if (CLK_BUF2 > 0)
				{
					insert_clkbuf2 = Clockpath_Buffer2(CLK_BUF3, CLK_BUF2, CTS_all_clk_route_info[i][j][CTS_all_clk_route_info[i][j].size() - 2], CTS_all_clk_route_info[i][j][CTS_all_clk_route_info[i][j].size() - 1]);
					clockpath_delay[index] += CLK_BUF2 * 0.072;
				}
				//	clock route���� buffer insertion ����
				for (int a = 0; a < insert_clkbuf2.size(); a++)
					CTS_all_clk_route_info[i][j].push_back(insert_clkbuf2[a]);
				//	input�� ���� ��� ������ �� �����Ƿ�, delay ����


				// CLKBUF1 insertion
				cout << endl << "index : " << index << " DFFSR_maxdelay : " << clockpath_delay[index] << endl;
				int CLK_BUF1 = (max_clockpath_delay - clockpath_delay[index]) / 0.058;	// CLK_BUF1�� ���� 

				if (CLK_BUF1 < 0)
					CLK_BUF1 = 0;
				else
					clock_skew = clock_skew - CLK_BUF1 * 0.058;	// clock skew ����

				cout << "CLK_BUF1 : " << CLK_BUF1 << endl;
				CLK_BUF_NUM += CLK_BUF1;

				vector<Gate>insert_clkbuf1;
				if (CLK_BUF1 > 0)
				{
					insert_clkbuf1 = Clockpath_Buffer1(CLK_BUF3, CLK_BUF2, CLK_BUF1, CTS_all_clk_route_info[i][j][CTS_all_clk_route_info[i][j].size() - 2], CTS_all_clk_route_info[i][j][CTS_all_clk_route_info[i][j].size() - 1]);
					clockpath_delay[index] += CLK_BUF1 * 0.058;
				}
				//	clock route���� buffer insertion ����
				for (int a = 0; a < insert_clkbuf1.size(); a++)
					CTS_all_clk_route_info[i][j].push_back(insert_clkbuf1[a]);
				//	input�� ���� ��� ������ �� �����Ƿ�, delay ����

				vector<Gate>insert_clkbuf;
				for (auto a : insert_clkbuf3)
					insert_clkbuf.push_back(a);
				for (auto a : insert_clkbuf2)
					insert_clkbuf.push_back(a);
				for (auto a : insert_clkbuf1)
					insert_clkbuf.push_back(a);

				insert_clkbuffers.push_back(insert_clkbuf);		// clock�� �������� ����Ǿ� ����
			}
		}
	}

	cout << "=============================After clock buffer insertion=============================" << endl;
	cout << "[" << endl;
	for (int i = 0; i < clockpath_delay.size(); i++)
	{
		cout << clockpath_delay[i] << "," << endl;
		fflush(stdin);
	}cout << "]" << endl;
	cout << "clockpath_delay.size() : " << clockpath_delay.size() << endl;
	return insert_clkbuffers;
}

pair<string, string> OpenTimerParser::buffer_getinput(unordered_map<string, string>cellpin2net)
{
	for (auto a : cellpin2net)
		if (a.first == "A")
		{
			//cout << "a.frist : " << a.first << " a.second : " << a.second << endl;
			return a;
		}
}


vector<Gate> OpenTimerParser::Clockpath_Buffer3(int clkbuf3, Gate front_Gate, Gate rear_Gate)
{
	vector<Gate> CLKBUF3_;
	Gate clkbuf;
	clkbuf.name = "";
	clkbuf.cell = "";

	//	������ ù CLKBUF ����
	pair<string, string> rear_gate_clkport = get_clock_input_Gate(rear_Gate.cellpin2net);	// CLK port�� net ���� ��������
	pair<string, string> front_gate_output = get_output_Gate(front_Gate.cellpin2net);		// �մ��� output ���� ��������

	pair<string, string> first_clkbuf_input = { "A", front_gate_output.second };			//	CLKBUF Y port�� ���� == ���Ե� CLKBUF A port ����
	pair<string, string> first_clkbuf_output = { "Y","clock_bf3__" + to_string(CLK_BUF_NUM - clkbuf3) };	//	���Ե� CLKBUF Y port ���� = ���� �����

	clkbuf.name = "CLKBUF3";
	clkbuf.cell = "CLKBUF3__" + to_string(CLK_BUF_NUM - clkbuf3);
	clkbuf.cellpin2net["A"] = first_clkbuf_input.second;
	clkbuf.cellpin2net["Y"] = first_clkbuf_output.second;

	// ������ output port�� net name ���� CLKBUF Y port == DFF�� clk port
	rear_Gate.cellpin2net["CLK"] = first_clkbuf_output.second;	// ���� all_clk_route�� �����ϴ� DFF�� port ����

	// ���� module instance CLK port ����
	for (size_t i = 0; i < module.gates.size(); i++)
	{
		if (module.gates[i].cell == rear_Gate.cell)
		{
			module.gates[i].cellpin2net["CLK"] = first_clkbuf_output.second;
		}
	}

	CLKBUF3_.push_back(clkbuf);

	//	������ ù Buffer ����
	if ((CLK_BUF_NUM - clkbuf3) == 0)
	{
		for (auto a : CLKBUF3_)
		{
			cout << "gate : " << a.name << endl;
			cout << "cell : " << a.cell << endl;
			for (auto b : a.cellpin2net)
			{
				cout << "pin : " << b.first << " " << b.second << endl;
				fflush(stdin);
			}
		}
		CTS_CLKBUF.push_back(CLKBUF3_);
		return CLKBUF3_;
	}
	else
	{
		for (int i = clkbuf3 - 1; i > 0; i--)
		{
			if (i == 1)
			{
				//	������ DFF CLK port �ٲ��ֱ�
				rear_Gate.cellpin2net["CLK"] = "clock_bf3__" + to_string(CLK_BUF_NUM - clkbuf3);
				for (auto a : rear_Gate.cellpin2net)
					cout << "a : " << a.first << "  " << a.second << endl;

				// ���� module instance CLK port ����
				for (size_t i = 0; i < module.gates.size(); i++)
				{
					if (module.gates[i].cell == rear_Gate.cell)
					{
						module.gates[i].cellpin2net["CLK"] = first_clkbuf_output.second;
					}
				}
			}
			pair<string, string> clkbuf_input = buffer_getinput(clkbuf.cellpin2net);	//������ CLKBUF�� output�� == ���Ե� CLKBUF�� input��
			pair<string, string> clkbuf_output = { "Y","clock_bf3__" + to_string(CLK_BUF_NUM - i) };	// ���Ե� CLKBUF�� output��
			cout << "clkbuf_output.first : " << clkbuf_output.first << "clkbuf_output.second : " << clkbuf_output.second << endl;
			clkbuf.name = "CLKBUF3";
			clkbuf.cell = "CLKBUF3__" + to_string(CLK_BUF_NUM - i);
			clkbuf.cellpin2net["A"] = clkbuf_input.second;
			clkbuf.cellpin2net["Y"] = clkbuf_output.second;
			CLKBUF3_.push_back(clkbuf);
		}
	}

	// ��� Ȯ����Ʈ
	for (auto a : CLKBUF3_)
	{
		cout << "gate : " << a.name << endl; fflush(stdin);
		cout << "cell : " << a.cell << endl; fflush(stdin);
		for (auto b : a.cellpin2net)
		{
			cout << "pin : " << b.first << " " << b.second << endl; fflush(stdin);
		}
	}

	cout << "gate : " << rear_Gate.name << endl; fflush(stdin);
	cout << "cell : " << rear_Gate.cell << endl; fflush(stdin);
	for (auto b : rear_Gate.cellpin2net)
	{
		cout << "pin : " << b.first << " " << b.second << endl; fflush(stdin);
	}


	CTS_CLKBUF.push_back(CLKBUF3_);
	PRE_CTS_buffers.push_back(CLKBUF3_);
	return CLKBUF3_;
}

vector<Gate> OpenTimerParser::Clockpath_Buffer2(int clkbuf3, int clkbuf2, Gate front_Gate, Gate rear_Gate)
{
	vector<Gate> CLKBUF2_;
	Gate clkbuf;
	clkbuf.name = "";
	clkbuf.cell = "";

	//	������ ù CLKBUF ����
	pair<string, string> rear_gate_output = get_clock_input_Gate(rear_Gate.cellpin2net);	// CLK port�� net ���� ��������
	pair<string, string> front_gate_output = get_output_Gate(front_Gate.cellpin2net);		// �մ��� output ���� ��������

	pair<string, string> first_clkbuf_input = { "A", front_gate_output.second };
	pair<string, string> first_clkbuf_output = { "Y","clock_bf2__" + to_string(CLK_BUF_NUM - clkbuf2) };
	clkbuf.name = "CLKBUF2";
	clkbuf.cell = "CLKBUF2__" + to_string(CLK_BUF_NUM - clkbuf2);
	clkbuf.cellpin2net["A"] = first_clkbuf_input.second;
	clkbuf.cellpin2net["Y"] = first_clkbuf_output.second;

	// ������ output port�� net name ���� CLKBUF Y port == DFF�� clk port
	rear_Gate.cellpin2net["CLK"] = first_clkbuf_output.second;	// ���� all_clk_route�� �����ϴ� DFF�� port ����
	// ���� module instance CLK port ����
	for (size_t i = 0; i < module.gates.size(); i++)
	{
		if (module.gates[i].cell == rear_Gate.cell)
		{
			module.gates[i].cellpin2net["CLK"] = first_clkbuf_output.second;
		}
	}

	CLKBUF2_.push_back(clkbuf);

	//	������ ù Buffer ����
	if ((CLK_BUF_NUM - clkbuf3 - clkbuf2) == 0)
	{
		for (auto a : CLKBUF2_)
		{
			cout << "gate : " << a.name << endl; fflush(stdin);
			cout << "cell : " << a.cell << endl; fflush(stdin);
			for (auto b : a.cellpin2net)
			{
				cout << "pin : " << b.first << " " << b.second << endl; fflush(stdin);
			}
		}
		CTS_CLKBUF.push_back(CLKBUF2_);
		return CLKBUF2_;
	}
	else
	{
		for (int i = clkbuf2 - 1; i > 0; i--)
		{
			if (i == 1)
			{
				rear_Gate.cellpin2net["CLK"] = "clock_bf2__" + to_string(CLK_BUF_NUM - clkbuf2);
				for (auto a : rear_Gate.cellpin2net)
					cout << "a : " << a.first << "  " << a.second << endl;

				// ���� module instance CLK port ����
				for (size_t i = 0; i < module.gates.size(); i++)
				{
					if (module.gates[i].cell == rear_Gate.cell)
					{
						module.gates[i].cellpin2net["CLK"] = first_clkbuf_output.second;
					}
				}
			}

			pair<string, string> clkbuf_input = buffer_getinput(clkbuf.cellpin2net);
			pair<string, string> clkbuf_output = { "Y","clock_bf2__" + to_string(CLK_BUF_NUM - i) };
			cout << "clkbuf_output.first : " << clkbuf_output.first << "clkbuf_output.second : " << clkbuf_output.second << endl; fflush(stdin);
			clkbuf.name = "CLKBUF2";
			clkbuf.cell = "CLKBUF2__" + to_string(CLK_BUF_NUM - i);
			clkbuf.cellpin2net["A"] = clkbuf_input.second;
			clkbuf.cellpin2net["Y"] = clkbuf_output.second;
			CLKBUF2_.push_back(clkbuf);
		}
	}

	// ��� Ȯ����Ʈ
	for (auto a : CLKBUF2_)
	{
		cout << "gate : " << a.name << endl; fflush(stdin);
		cout << "cell : " << a.cell << endl; fflush(stdin);
		for (auto b : a.cellpin2net)
		{
			cout << "pin : " << b.first << " " << b.second << endl;
			fflush(stdin);
		}

	}

	cout << "gate : " << rear_Gate.name << endl; fflush(stdin);
	cout << "cell : " << rear_Gate.cell << endl; fflush(stdin);
	for (auto b : rear_Gate.cellpin2net)
	{
		cout << "pin : " << b.first << " " << b.second << endl; fflush(stdin);
	}

	PRE_CTS_buffers.push_back(CLKBUF2_);
	CTS_CLKBUF.push_back(CLKBUF2_);
	return CLKBUF2_;
}

vector<Gate> OpenTimerParser::Clockpath_Buffer1(int clkbuf3, int clkbuf2, int clkbuf1, Gate front_Gate, Gate rear_Gate)
{
	vector<Gate> CLKBUF1_;
	Gate clkbuf;
	clkbuf.name = "";
	clkbuf.cell = "";

	//	������ ù CLKBUF ����
	pair<string, string> rear_gate_output = get_clock_input_Gate(rear_Gate.cellpin2net);	// CLK port�� net ���� ��������
	pair<string, string> front_gate_output = get_output_Gate(front_Gate.cellpin2net);		// �մ��� output ���� ��������

	pair<string, string> first_clkbuf_input = { "A",front_gate_output.second };
	pair<string, string> first_clkbuf_output = { "Y","clock_bf1__" + to_string(CLK_BUF_NUM - clkbuf1) };
	clkbuf.name = "CLKBUF1";
	clkbuf.cell = "CLKBUF1__" + to_string(CLK_BUF_NUM - clkbuf1);
	clkbuf.cellpin2net["A"] = first_clkbuf_input.second;
	clkbuf.cellpin2net["Y"] = first_clkbuf_output.second;


	// ������ output port�� net name ���� CLKBUF Y port == DFF�� clk port
	rear_Gate.cellpin2net["CLK"] = first_clkbuf_output.second;	// ���� all_clk_route�� �����ϴ� DFF�� port ����

	// ���� module instance CLK port ����
	for (size_t i = 0; i < module.gates.size(); i++)
	{
		if (module.gates[i].cell == rear_Gate.cell)
		{
			module.gates[i].cellpin2net["CLK"] = first_clkbuf_output.second;
		}
	}

	CLKBUF1_.push_back(clkbuf);

	//	������ ù Buffer ����
	if ((CLK_BUF_NUM - clkbuf3 - clkbuf2) == 0)
	{
		for (auto a : CLKBUF1_)
		{
			cout << "gate : " << a.name << endl; fflush(stdin);
			cout << "cell : " << a.cell << endl; fflush(stdin);
			for (auto b : a.cellpin2net)
			{
				cout << "pin : " << b.first << " " << b.second << endl; fflush(stdin);
			}
		}
		CTS_CLKBUF.push_back(CLKBUF1_);
		return CLKBUF1_;
	}
	else
	{
		for (int i = clkbuf1 - 1; i > 0; i--)
		{
			if (i == 1)
			{
				//	������ DFF CLK port �ٲ��ֱ�
				rear_Gate.cellpin2net["CLK"] = "clock_bf1__" + to_string(CLK_BUF_NUM - clkbuf1);
				for (auto a : rear_Gate.cellpin2net)
					cout << "a : " << a.first << "  " << a.second << endl;

				// ���� module instance CLK port ����
				for (size_t i = 0; i < module.gates.size(); i++)
				{
					if (module.gates[i].cell == rear_Gate.cell)
					{
						module.gates[i].cellpin2net["CLK"] = first_clkbuf_output.second;
					}
				}
			}
			pair<string, string> clkbuf_input = buffer_getinput(clkbuf.cellpin2net);
			pair<string, string> clkbuf_output = { "Y","clock_bf1__" + to_string(CLK_BUF_NUM - i) };

			cout << "clkbuf_output.first : " << clkbuf_output.first << "clkbuf_output.second : " << clkbuf_output.second << endl; fflush(stdin);
			clkbuf.name = "CLKBUF1";
			clkbuf.cell = "CLKBUF1__" + to_string(CLK_BUF_NUM - i);
			clkbuf.cellpin2net["A"] = clkbuf_input.second;
			clkbuf.cellpin2net["Y"] = clkbuf_output.second;
			CLKBUF1_.push_back(clkbuf);
		}
	}

	// ��� Ȯ����Ʈ
	for (auto a : CLKBUF1_)
	{
		cout << "gate : " << a.name << endl; fflush(stdin);
		cout << "cell : " << a.cell << endl; fflush(stdin);
		for (auto b : a.cellpin2net)
		{
			cout << "pin : " << b.first << " " << b.second << endl; fflush(stdin);
		}
	}

	cout << "gate : " << rear_Gate.name << endl; fflush(stdin);
	cout << "cell : " << rear_Gate.cell << endl; fflush(stdin);
	for (auto b : rear_Gate.cellpin2net)
	{
		cout << "pin : " << b.first << " " << b.second << endl; fflush(stdin);
	}
	PRE_CTS_buffers.push_back(CLKBUF1_);
	CTS_CLKBUF.push_back(CLKBUF1_);
	return CLKBUF1_;
}



//	input2DFF���� max route ã��, max_route�� index�� returns
void OpenTimerParser::input2DFF_route_delay_sum()
{
	DFF_delay.resize(DFFSR_COUNT);	// DFFSR count��ŭ DFFSR ����� max delay �����ϱ�

	for (size_t i = 0; i < all_input2DFF_route.size(); i++)
	{
		vector<float>temp_delay;	// input1�� �� �����ϴ� route(2���� �迭) 1���� route delay ���, input 1�� �� �����ϴ� delay �迭
		for (size_t j = 0; j < all_input2DFF_route[i].size(); j++)
		{
			float delay = 0;
			int size = all_input2DFF_route[i][j].size() - 1;
			int index = stoi(all_input2DFF_route[i][j][size].cell.substr(6)) - 1;	// DFFSR_maxdelay�� �ش��ϴ� index == DFFSR ���ڿ��� 1�� ����
			for (size_t k = 0; k < all_input2DFF_route[i][j].size() - 1; k++)
			{
				//	DFFSR �����ϰ� delay ���
				vector<float>tmp_gate = stdlibrary_180nm(all_input2DFF_route[i][j][k].name);
				delay += tmp_gate[0];
			}
			// DFF delay update
			if (DFF_delay[index] < delay)
				DFF_delay[index] = delay;
		}
	}
}

/*
//	max route �������� Buffer insertion �����ϱ�
void OpenTimerParser::input2DFF_Bufferinsertion()
{
	cout << "////////////////////////////////////////////////////// input2DFF Bufferinsertion/////////////////////////////////////////////////////////////" << endl;
	vector<vector<Gate>> input2DFF_buffers;		// input2DFF path���� buffer insertion
	input2DFF_route_delay_sum();	// input2DFF �� route delay ���ϱ�

	
	//cout << "------------------------------------------------------------------------" << endl;
	//cout << "Before Buffer insertion : " << endl;
	//standard_deviation(route_delay, max_route[1]);

	//	buffer insertion���� ���� �ٲ�� input part  --> ���� �ٲ��??
	for (auto a : module.inputs)
		pre_buffer_inputs.push_back(a);
	
	for (size_t i = 0; i < all_input2DFF_route.size(); i++)
	{
		for (size_t j = 0; j < all_input2DFF_route[i].size(); j++)
		{
			if (input2DFF_delay[i][j] < max_route && input2DFF_delay[i][j]>0)
			{
				cout << "index : " << i <<", " << j << " input2DFF_delay : " << input2DFF_delay[i][j] << endl;
				int bufferX2 = (max_route - input2DFF_delay[i][j]) / 0.071;	// bufferX2�� ���� 
				if (bufferX2 < 0)
					bufferX2 = 0;
				cout << "bufferX2 : " << bufferX2 << endl;
				input2DFF_NUM_BUFFER += bufferX2;

				int size = all_input2DFF_route[i][j].size();
				
				vector<Gate>insert_bufferX2;
				if (bufferX2 > 0)
				{
					insert_bufferX2 = BufferX2(bufferX2, all_input2DFF_route[i][j][ size-2], all_input2DFF_route[i][j][size - 1]);
					input2DFF_delay[i][j] += bufferX2 * 0.071;
				}
				//	route���� buffer insertion ����
				for (int a = 0; a < insert_bufferX2.size(); a++)
					all_input2DFF_route[i][j].push_back(insert_bufferX2[a]);
				//	input�� ���� ��� ������ �� �����Ƿ�, delay ����
				

				//	bufferX4 insertion
				vector<Gate>insert_bufferX4;
				int bufferX4 = (max_route - input2DFF_delay[i][j] - bufferX2 * 0.071) / 0.065;
				if (bufferX4 < 0)
					bufferX4 = 0;
				cout << "bufferX4 : " << bufferX4 << endl;
				input2DFF_NUM_BUFFER += bufferX4;
				
				if (bufferX4 > 0)
				{
					insert_bufferX4 = BufferX4(bufferX2, bufferX4, all_input2DFF_route[i][j][all_input2DFF_route[i][j].size() - 2], all_input2DFF_route[i][j][all_input2DFF_route[i][j].size() - 1]);
					input2DFF_delay[i][j] += bufferX4 * 0.065;
				}
				for (int a = 0; a < insert_bufferX4.size(); a++)
					all_input2DFF_route[i][j].push_back(insert_bufferX4[a]);
				
				vector<Gate>insert_buffer;
				for (auto a : insert_bufferX2)
					insert_buffer.push_back(a);
				for (auto a : insert_bufferX4)
					insert_buffer.push_back(a);
		
				input2DFF_buffers.push_back(insert_buffer);		// input ���� ����Ǿ� �ֱ� ������, ������ ���� �̿��ؾ� ��
				
			}
		}
	}

	
	for (int i = 0; i < input2DFF_buffers.size(); i++)
	{
		for (auto a : input2DFF_buffers[i])
			cout << a.cell << "->";
		PRE_CTS_buffers.push_back(input2DFF_buffers[i]);
		cout << endl;
	}
	
	for (int i = 0; i < input2DFF_delay.size(); i++)
	{
		for (int j = 0; j < input2DFF_delay[i].size(); j++)
		{
			cout << "delay : " << input2DFF_delay[i][j] << " ";
		}
		cout << endl;
	}

	//cout << "------------------------------------------------------------------------" << endl;
	//cout << "After Buffer insertion : " << endl;
	//standard_deviation(route_delay, max_route[1]);
	return;
}
*/
int OpenTimerParser::isD_DFF(unordered_map<string, string>front_cell, unordered_map<string, string>last_cell)
{
	string portD;
	for (auto a : last_cell)
		if (a.first == "D")
			portD = a.second;

	for (auto a : front_cell)
	{
		if (a.second == portD)
			return 1;
		else
			return 0;
	}
}


vector<Gate> OpenTimerParser::BufferX2(int bufferX2, Gate front_Gate, Gate last_Gate)
{
	vector<Gate>BufferX2;
	Gate BUF, TMP;	// BUF�ʱ�ȭ
	BUF.name = "";
	BUF.cell = "";

	// ���� gate output�� DFF�� D port�� input�� ���� �ʴٸ� ����
	if (!isD_DFF(front_Gate.cellpin2net, last_Gate.cellpin2net))
		return BufferX2;

	if (bufferX2!=0)
	{
		pair<string, string> last_gate_output = get_output_Gate(last_Gate.cellpin2net);
		pair<string, string> first_buffer_input = { "A","b" + to_string(input2DFF_NUM_BUFFER - bufferX2) };
		pair<string, string> first_buffer_output = { "Y",last_gate_output.second };
		BUF.name = "BUFX2";
		BUF.cell = "BUFX2__" + to_string(input2DFF_NUM_BUFFER - bufferX2);
		BUF.cellpin2net["A"] = "b" + to_string(input2DFF_NUM_BUFFER - bufferX2);
		BUF.cellpin2net["Y"] = first_buffer_output.second;
		BufferX2.push_back(BUF);

		// ���� module instance port ����
		for (size_t i = 0; i < module.gates.size(); i++)
		{
			if (module.gates[i].cell == front_Gate.cell)
			{
				module.gates[i].cellpin2net["Y"] = first_buffer_input.second;
			}
		}

			if (bufferX2 == 1)
			{
				string tmp;
				for (auto a : front_Gate.cellpin2net)
					if (a.first == "Y")
						tmp = a.first;

				front_Gate.cellpin2net[tmp] = "b" + to_string(input2DFF_NUM_BUFFER - bufferX2);
				for (auto a : front_Gate.cellpin2net)
					cout << "a : " << a.first << "  " << a.second << endl;
			}
			else
			{
				for (int i = bufferX2 - 1; i > 0; i--)
				{
					if (i == bufferX2 - 1)
					{
						string tmp;
						for (auto a : front_Gate.cellpin2net)
							if (a.first == "Y")
								tmp = a.first;

						front_Gate.cellpin2net[tmp] = "b" + to_string(input2DFF_NUM_BUFFER - bufferX2);
						for (auto a : front_Gate.cellpin2net)
							cout << "a : " << a.first << "  " << a.second << endl;

					}
					pair<string, string> buffer_input = { "A","b" + to_string(input2DFF_NUM_BUFFER - i) };
					pair<string, string> buffer_output = buffer_getinput(BUF.cellpin2net);
					buffer_output.first = "Y";
					cout << "buffer_output.first : " << buffer_output.first << " buffer_output.second : " << buffer_output.second << endl;
					BUF.name = "BUFX2";
					BUF.cell = "BUFX2__" + to_string(input2DFF_NUM_BUFFER - i);
					BUF.cellpin2net["A"] = "b" + to_string(input2DFF_NUM_BUFFER - i);
					BUF.cellpin2net["Y"] = buffer_output.second;
					//BUF.cellpin2net.insert(buffer_input);
					//BUF.cellpin2net.insert(buffer_output);
					BufferX2.push_back(BUF);
				}
			}
	}

	for (auto a : BufferX2)
	{
		cout << "name : " << a.name << endl;
		cout << "cell : " << a.cell << endl;
		for (auto b : a.cellpin2net)
			cout << b.first << "  " << b.second << endl;
	}
	
	return BufferX2;
}


vector<Gate> OpenTimerParser::BufferX4(int bufferX2, int bufferX4, Gate front_Gate, Gate last_Gate)
{
	vector<Gate>BufferX4;
	Gate BUF;	// BUF�ʱ�ȭ
	BUF.name = "";
	BUF.cell = "";
	if (bufferX4 != 0)
	{
		cout << " module input == last gate input " << endl;
		//	 module input == last gate input : output �� �ʿ��� Buffer insertion ����.
		//	������ ù Buffer ����
		pair<string, string> last_gate_output = get_output_Gate(last_Gate.cellpin2net);
		pair<string, string> first_buffer_input = { "A","b" + to_string(input2DFF_NUM_BUFFER - bufferX4) };
		pair<string, string> first_buffer_output = { "Y",last_gate_output.second };
		BUF.name = "BUFX4";
		BUF.cell = "BUFX4__" + to_string(input2DFF_NUM_BUFFER - bufferX4);
		BUF.cellpin2net["A"] = "b" + to_string(input2DFF_NUM_BUFFER - bufferX4);
		BUF.cellpin2net["Y"] = first_buffer_output.second;

		// ���� module instance port ����
		for (size_t i = 0; i < module.gates.size(); i++)
		{
			if (module.gates[i].cell == front_Gate.cell)
			{
				module.gates[i].cellpin2net["Y"] = first_buffer_input.second;
			}
		}

		BufferX4.push_back(BUF);

		if (bufferX4 == 1)
		{
			string tmp;
			for (auto a : front_Gate.cellpin2net)
				if (a.first == "Y")
					tmp = a.first;

			front_Gate.cellpin2net[tmp] = "b" + to_string(input2DFF_NUM_BUFFER - bufferX4);
			for (auto a : front_Gate.cellpin2net)
				cout << "a : " << a.first << "  " << a.second << endl;
		}
		else
		{
			for (int i = bufferX4 - 1; i > 0; i--)
			{
				if (i == bufferX4 - 1)
				{
					string tmp;
					for (auto a : front_Gate.cellpin2net)
						if (a.first == "Y")
							tmp = a.first;

					front_Gate.cellpin2net[tmp] = "b" + to_string(input2DFF_NUM_BUFFER - bufferX4);
					for (auto a : front_Gate.cellpin2net)
						cout << "a : " << a.first << "  " << a.second << endl;

				}
				pair<string, string> buffer_input = { "A","b" + to_string(input2DFF_NUM_BUFFER - i) };
				pair<string, string> buffer_output = buffer_getinput(BUF.cellpin2net);
				buffer_output.first = "Y";
				cout << "buffer_output.first : " << buffer_output.first << " buffer_output.second : " << buffer_output.second << endl;
				BUF.name = "BUFX4";
				BUF.cell = "BUFX4__" + to_string(input2DFF_NUM_BUFFER - i);
				BUF.cellpin2net["A"] = "b" + to_string(input2DFF_NUM_BUFFER - i);
				BUF.cellpin2net["Y"] = buffer_output.second;
				//BUF.cellpin2net.insert(buffer_input);
				//BUF.cellpin2net.insert(buffer_output);
				BufferX4.push_back(BUF);
			}
		}
	}
	
	for (auto a : BufferX4)
	{
		cout << "name : " << a.name << endl;
		cout << "cell : " << a.cell << endl;
		for (auto b : a.cellpin2net)
			cout << b.first << "  " << b.second << endl;
	}
	
	return BufferX4;
}

void OpenTimerParser::PRE_CTS_PRINT()
{
	//input2DFF_Bufferinsertion();
	cout << "module " << module.name << "(" << endl;
	for (auto a : module.verilog_inputs)
		cout << "  input " << a << "," << endl;
	for (int i=0; i<module.verilog_outputs.size()-1; i++)
		cout << "  output " << module.verilog_outputs[i] << "," << endl;
	cout << "  output " << module.verilog_outputs[module.verilog_outputs.size() - 1] << endl;
	cout << ");" << endl;

	for (auto a : module.gates)
	{
		cout << "  " << a.name << " " << a.cell << " (";
		for (auto b : a.cellpin2net)
			if (b.first!="gnd" && b.first!="vdd")
				cout << "." << b.first << "(" << b.second << "), ";
		cout << "\b\b);" << endl;
	}

	for (int i = 0; i < PRE_CTS_buffers.size(); i++)
	{
		for (int j = 0; j < PRE_CTS_buffers[i].size(); j++)
		{
			// ���� verilog instance �߰�
			module.gates.push_back(PRE_CTS_buffers[i][j]);
			cout << "  " << PRE_CTS_buffers[i][j].name << " " << PRE_CTS_buffers[i][j].cell << " (";
			for (auto b : PRE_CTS_buffers[i][j].cellpin2net)
				cout << "." << b.first << "(" << b.second << "), ";
			cout << "\b\b);" << endl;
		}
	}

	cout << "endmodule"<<endl;
	cout << endl;
}


// Pre-CTS ���� ������ϱ�

void OpenTimerParser::PRE_CTS_FILE_PRINT()
{
	cout << "====================================== CTS_result.v ������ �����Ǿ����ϴ�. ===================================" << endl;
	ofstream fout("CTS_result.v", std::ios::out);
	if (fout.is_open())
	{
		fout << "module " << module.name << "(" << "\n";
		for (auto a : pre_buffer_inputs)
			fout << "  input " << a << "," << "\n";
		for (auto a : module.outputs)
			fout << "  output " << a << "\n";
		fout << ");" << "\n";

		for (auto a : module.gates)
		{
			fout << "  " << a.name << " " << a.cell << " (";
			for (auto b : a.cellpin2net)
				fout << "." << b.first << "(" << b.second << "), ";
			fout << "\b\b);" << "\n";
		}

		for (int i = 0; i < PRE_CTS_buffers.size(); i++)
		{
			for (int j = 0; j < PRE_CTS_buffers[i].size(); j++)
			{
				fout << "  " << PRE_CTS_buffers[i][j].name << " " << PRE_CTS_buffers[i][j].cell << " (";
				for (auto b : PRE_CTS_buffers[i][j].cellpin2net)
					fout << "." << b.first << "(" << b.second << "), ";
				fout << "\b\b);" << "\n";
			}
		}
	}
	fout << "endmodule" << "\n";
	fout << "\n";
}






void OpenTimerParser::standard_deviation(vector<float> route_delay, float max_delay)
{
	float sum = std::accumulate(route_delay.begin(), route_delay.end(), 0.0);
	float mean = sum / route_delay.size();

	vector<float> diff(route_delay.size());
	transform(route_delay.begin(), route_delay.end(), diff.begin(), [mean](float x) { return x - mean; });
	float sq_sum = inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
	float stdev = sqrt(sq_sum / route_delay.size());
	cout << "------------------------------------------------------------------------" << endl;
	cout << "max_delay : " << max_delay << endl;
	cout << "mean : " << mean << endl;
	cout << "stdev : " << stdev << endl;
	cout << "------------------------------------------------------------------------" << endl;
}

// std cell library TSMC 180nm 
const vector<float> OpenTimerParser::stdlibrary_180nm(string gate_name)
{
	std::unordered_map<string, vector<float>> stdlib_gate;
	//	tuple (delay, area, gate_input_num)
	stdlib_gate["AND2X1"] = { 0.090, 32, 2 };
	stdlib_gate["AND2X2"] = { 0.084, 32, 2 };
	stdlib_gate["AOI21X1"] = { 0.041, 32, 3 };

	stdlib_gate["AOI22X1"] = { 0.053, 40, 4 };

	stdlib_gate["BUFX2"] = { 0.071, 24, 1 };
	stdlib_gate["BUFX4"] = { 0.065, 32, 1 };

	stdlib_gate["CLKBUF1"] = { 0.058, 72, 1 };
	stdlib_gate["CLKBUF2"] = { 0.072, 104, 1 };
	stdlib_gate["CLKBUF3"] = { 0.079, 136, 1 };

	stdlib_gate["DFFPOSX1"] = { 0.166, 96, 1 };		// clock port ����
	stdlib_gate["DFFNEGX1"] = { 0.236, 96, 1 };		// clock port ����
	stdlib_gate["DFFSR"] = { 0.221, 176, 3 };		// clock port ����

	stdlib_gate["INVX1"] = { 0.019, 16, 1 };
	stdlib_gate["INVX2"] = { 0.016, 16, 1 };
	stdlib_gate["INVX4"] = { 0.015, 24, 1 };
	stdlib_gate["INVX8"] = { 0.015, 40, 1 };

	//stdlib_gate["LATCH"] = (0, 0, );

	stdlib_gate["MUX2X1"] = { 0.109, 48, 3 };

	stdlib_gate["NAND2X1"] = { 0.025, 24, 2 };

	stdlib_gate["NAND3X1"] = { 0.036, 36, 3 };

	stdlib_gate["NOR2X1"] = { 0.031, 24, 2 };
	stdlib_gate["NOR3X1"] = { 0.048, 64, 3 };

	stdlib_gate["OAI21X1"] = { 0.043, 23, 3 };
	stdlib_gate["OAI22X1"] = { 0.053, 40, 4 };

	stdlib_gate["OR2X1"] = { 0.103, 32, 2 };
	stdlib_gate["OR2X2"] = { 0.099, 32, 2 };

	stdlib_gate["TBUFX1"] = { 0.109, 40, 2 };
	stdlib_gate["TBUFX2"] = { 0.106, 56, 2 };

	stdlib_gate["XNOR2X1"] = { 0.125, 56, 2 };

	stdlib_gate["XOR2X1"] = { 0.123, 56, 2 };

	if (stdlib_gate.count(gate_name))
		return stdlib_gate[gate_name];
	else
		return {};
}
