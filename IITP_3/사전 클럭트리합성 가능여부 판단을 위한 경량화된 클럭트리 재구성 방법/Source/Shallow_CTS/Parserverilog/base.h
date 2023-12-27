#include "ot_parser.h"
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
		if (elem.second == "clock")
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
	for (auto a : Clock_Gates)
		cout << "cell name : " << a.cell << endl;
	return;
}


//	input port�� �����ϴ� gate ���� (input clock�� ��� �ִ� gate ����)
void OpenTimerParser::get_input_Module()
{
	for (size_t i = 0; i < module.gates.size(); i++)
	{
		for (auto a : module.gates[i].cellpin2net)
			if (nostrip_isinput(a) && a.second != "clock")
			{
				Input_Gates.push_back(module.gates[i]);
				break;
			}
	}
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
		if (a.first != "Y" && a.first != "Q" && a.first != "CLK")
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

float OpenTimerParser::Check_input_cell(Path p, vector<Gate> g)
{
	if (g[g.size() - 1].cell == p.out_)
		for (auto i : g[g.size() - 1].cellpin2net)
			if (i.second == p.in_ && i.first == p.out_port)
				return p.delay;
	return 0;
}

float OpenTimerParser::Check_in_out(Path p, vector<Gate> g)
{
	int cnt = 0;
	for (size_t i = 1; i < g.size(); i++)
	{
		if (g[i].cell == p.in_ && g[i - 1].cell == p.out_)
		{
			//cout << g[i].cellpin2net[p.in_port] << g[i - 1].cellpin2net[p.out_port] << endl;
			if (g[i].cellpin2net[p.in_port] == g[i - 1].cellpin2net[p.out_port])
				return p.delay;
		}
	}
	return 0;
}

float OpenTimerParser::Check_cell_out(Path p, vector<Gate> g)
{
	if (g[0].cell == p.in_)
		for (auto i : g[0].cellpin2net)
			if (i.second == p.out_)
				return p.delay;
	return 0;
}

//	data path Ž���� �� children ���� ��� output-gate-input <---> output-gate-input �̷��� ����Ǿ�� ��
vector<Gate> OpenTimerParser::getchildren_Gate(const unordered_map<string, string>& pin2net, pair<string, string> gate_output)
{
	vector<Gate> get_children;
	for (int i = 0; i < module.gates.size(); i++)
		for (auto n : module.gates[i].cellpin2net)
			if (n.first == "Q" || n.first == "Y")
				for (auto elem : pin2net)
					if (elem.first != "Q" && elem.first != "Y" && elem.first != "vdd" && elem.first != "gnd" && elem.first != "CLK" && module.gates[i].name.find("CLKBUF") == string::npos)
						if (elem.second == n.second)
							get_children.push_back(module.gates[i]);
	// get_children ���� Ȯ���ϱ�
	/*
	for (auto a : get_children)
		cout << a.cell << " ";
	cout << endl;
	*/
	return get_children;
}

//	CLK path Ž���� �� children ���� ��� input - gate - clk <---> clk - gate - output �̷��� ����Ǿ�� ��
vector<Gate> OpenTimerParser::getchildren_CLK_Gate(const unordered_map<string, string>& pin2net, pair<string, string> clk_port)
{
	vector<Gate> get_clk_children;
	for (int i = 0; i < module.gates.size(); i++)
		for (auto n : module.gates[i].cellpin2net)
			if (n.first == "CLK")
				if (clk_port.second == n.second)
					get_clk_children.push_back(module.gates[i]);
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
			if (n.first != "Q" && n.first != "Y" && n.first != "vdd" && n.first != "gnd" && n.first != "CLK" && module.gates[i].name.find("CLKBUF") == string::npos)
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
