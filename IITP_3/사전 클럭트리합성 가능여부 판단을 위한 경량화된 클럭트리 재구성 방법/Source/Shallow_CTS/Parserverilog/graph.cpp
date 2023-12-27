#include "Sampleparser.h"

string Sampleparser::getoutput()
{
	return output;
}

bool Sampleparser::isinput(string name)
{
	for (size_t i = 0; i < inputs.size(); i++)
		if (name == inputs[i])
			return true;
	return false;
}

string Sampleparser::getmodule(string name)
{
	for (size_t i = 0; i < insts.size(); i++)
	{
		string lastnetname = std::get<std::string>(insts[i].net_names[0][0]);
		if (lastnetname == name)
		{
			return insts[i].module_name;
		}
	}
	return  "()";
}

vector<string> Sampleparser::getchildren(string name)
{
	vector<string> children;
	for (size_t i = 0; i < insts.size(); i++)
	{
		string lastnetname = std::get<std::string>(insts[i].net_names[0][0]);
		if (lastnetname == name)
		{
			for (size_t n = 1; n <=insts[i].net_names.size() - 1; n++) {
				children.push_back(std::get<std::string>(insts[i].net_names[n][0]));
			}
			break;
		}
	}
	return children;
}

int Sampleparser::Gatedelay(std::string module_name)
{
	//int delay = 0;
	std::map<std::string, InstGateDelay> DelayList;
	DelayList["AND2"] = InstGateDelay::and2;
	DelayList["OR2"] = InstGateDelay::or2;
	DelayList["INV"] = InstGateDelay::inv;
	DelayList["XOR2"] = InstGateDelay::xor2;
	DelayList["NXOR2"] = InstGateDelay::nxor2;
	DelayList["NAND2"] = InstGateDelay::nand2;
	DelayList["NOR2"] = InstGateDelay::nor2;
	DelayList["AND3"] = InstGateDelay::and3;
	DelayList["OR3"] = InstGateDelay::or3;
	DelayList["XOR3"] = InstGateDelay::xor3;
	DelayList["NXOR3"] = InstGateDelay::nxor3;
	DelayList["NAND3"] = InstGateDelay::nand3;
	DelayList["NOR3"] = InstGateDelay::nor3;

	return (int)DelayList[module_name];
}



void Sampleparser::traversal()
{
	traversal(getoutput(), 0);
}

void Sampleparser::traversal(string name, int level)
{
	for (int i = 0; i < level; i++)
	{
		cout << "----  ";
	}
	cout << name << " ";
	cout << Gatedelay(getmodule(name)) << " ";
	cout << getmodule(name) << endl;

	int myvalue = Gatedelay(getmodule(name));
	if (isinput(name))
	{
		return;
	}

	vector<string> children = getchildren(name);
	for (int i = 0; i < children.size(); i++)
	{
		traversal(children[i], level + 1);
	}

	return;
}



int Sampleparser::route()
{
	vector <string> strname;
	return route(getoutput(), strname);
}



int Sampleparser::route(string name, vector <string> strname)
{
	if (isinput(name))
	{
		int total = 0;
		cout << name << endl;
		for (int i = 0; i < strname.size(); i++)
		{
			total += Gatedelay(getmodule(strname[i]));
			cout << getmodule(strname[i]) << "(" << Gatedelay(getmodule(strname[i])) << ") ";
		}
		cout << ":" << total << endl;
		if (total == MaxDelay())
		{
			cout << "max delay route 입니다." << endl;
		}
		return 0;
	}

	strname.push_back(name);

	vector<string> children = getchildren(name);

	bool allchild = true;	// child가 모두 input일 경우
	for (int i = 0; i < children.size(); i++)
	{
		if (!isinput(children[i]))
		{
			allchild = false;
			break;
		}
	}

	for (int i = 0; i < children.size(); i++)
	{
		route(children[i], strname);
		if (allchild && isinput(children[i]))
			break;
	}

	return 0;
}

int Sampleparser::MaxDelay()
{
	return MaxDelay(getoutput());
}


int Sampleparser::MaxDelay(string name)
{
	int myvalue = Gatedelay(getmodule(name));
	if (isinput(name))
	{
		return 0;
	}

	vector<string> children = getchildren(name);

	int maxvalue = 0;
	for (int i = 0; i < children.size(); i++)
	{
		int value = MaxDelay(children[i]);
		if (maxvalue < value)
			maxvalue = value;
	}

	return myvalue + maxvalue;
}


int Sampleparser::addBuffer()
{
	vector <string> strname;
	return addBuffer(getoutput(), strname);
}

int Sampleparser::addBuffer(string name, vector <string> strname)
{
	int bufferdelay = 0;
	int buffer_cnt = 0;
	vector<string> children;
	if (isinput(name))
	{
		int total = 0;
		int i;
		
		for (i = 0; i < strname.size(); i++)
		{
			total += Gatedelay(getmodule(strname[i]));
			cout << getmodule(strname[i]) << "(" << Gatedelay(getmodule(strname[i])) << ") ";
		}
		string tmp = strname[i - 1];
		cout << endl;
		if (total < MaxDelay())
		{
			int diff = MaxDelay() - total;
			cout << endl <<"Buffer insertion을 진행합니다." << endl;
			bufferdelay = Bufferinsertion(tmp, diff);
			buffer_cnt = diff / bufferdelay;
			for (int j = 0; j < buffer_cnt; j++)
			{
				cout << " INV_" << bufferdelay << "(" << bufferdelay << ")";
			}
			cout << "\nbuffer " << buffer_cnt << "개 (" << bufferdelay << "ps) 삽입합니다." << endl;
		}
		else if (total == MaxDelay())
		{
			cout << "max delay route 입니다." << endl;
		}
		return 0;
	}

	strname.push_back(name);

	children = getchildren(name);

	bool allchild = true;	// child가 모두 input일 경우
	for (int i = 0; i < children.size(); i++)
	{
		if (!isinput(children[i]))
		{
			allchild = false;
			break;
		}
	}

	for (int i = 0; i < children.size(); i++)
	{
		addBuffer(children[i], strname);
		if (allchild && isinput(children[i]))
			break;
	}

	return 0;
}



int Sampleparser::Bufferinsertion(string name, int diff)
{
	/// 구조체 Buffer
	BUFFER Buffer;
	Buffer.route_output = name;

	int buffer = 0;	// 한 route에 삽입된 버퍼 개수
	int bufferdelay = 0;
	if (diff > 0)
	{
		if (diff <= 10)
		{
			bufferdelay = 2;
			buffer = diff / 2;
			Buffer.numOfbuffer = buffer;
		}
		else if (diff > 10 && diff <= 20)
		{
			bufferdelay = 4;
			buffer = diff / 4;
			Buffer.numOfbuffer = buffer;
		}
		else if (diff > 20 && diff <= 30)
		{
			bufferdelay = 6;
			buffer = diff / 6;
			Buffer.numOfbuffer = buffer;
		}
		else if (diff > 30)
		{
			bufferdelay = 8;
			buffer = diff / 8;
			Buffer.numOfbuffer = buffer;
		}
	}
	NUM_BUFFER += buffer;	// 한 루트당 총 버퍼 개수

	INSERT_BUFFER.push_back(Buffer);
	return bufferdelay;
}


/// Buffer insertion 후  instance 추가하기
void Sampleparser::INSERTION_INSTS()
{
	for (int i = before_NUM_BUFFER; i < NUM_BUFFER; i++)
	{
		add_insts.resize(4);
		ADD_INSTS.push_back(add_insts);
	}
	
	int before_value = 0;

	int i, j;
	for (i = 0; i < INSERT_BUFFER.size(); i++)
	{
		for (j = before_value; j < (int)(INSERT_BUFFER[i].numOfbuffer + before_value); j++)
		{
			ADD_INSTS[j][0] = "BUF";
			ADD_INSTS[j][1] = "B" + to_string(j + 1);
			ADD_INSTS[j][3] = add_wires[j];
			if (j == before_value)
			{
				ADD_INSTS[j][2] = INSERT_BUFFER[i].route_output;
				for (int m = 0; m < ORIGIN_INSTS.size(); m++)
				{
					for (int n = 0; n < ORIGIN_INSTS[m].size(); n++)
					{
						if (ORIGIN_INSTS[m][ORIGIN_INSTS[m].size() - 1] == ADD_INSTS[j][2])
							ORIGIN_INSTS[m][ORIGIN_INSTS[m].size() - 1] = "b" + to_string(INSERT_BUFFER[i].numOfbuffer + before_value);
					}
				}
			}
			else
			{
				ADD_INSTS[j][2] = ADD_INSTS[j - 1][3];
			}	
				
		}
		before_value = INSERT_BUFFER[i].numOfbuffer;
	}
}


////////// Add wire //////////
// 삽입된 buffer 개수만큼 wire 추가
void Sampleparser::ADD_WIRES()
{
	string tmp;	
	for (int i = before_NUM_BUFFER; i < NUM_BUFFER; i++)
	{
		tmp = "b" + to_string(i);
		add_wires.push_back(tmp);
	}
	
}



// 기존의 insts vector vector<string>으로 변환
void Sampleparser::Origin_Insts()
{
	for (size_t i = 0; i < insts.size(); i++)
	{
		origin_insts.resize(insts[i].net_names.size()+2);
		ORIGIN_INSTS.push_back(origin_insts);
	}
	for (int i = 0; i < ORIGIN_INSTS.size(); i++)
	{
		ORIGIN_INSTS[i][0] = (string)insts[i].module_name;
		ORIGIN_INSTS[i][1] = (string)insts[i].inst_name;
		for (int j = 2; j < ORIGIN_INSTS[i].size(); j++)
		{
			ORIGIN_INSTS[i][j] = std::get<std::string>(insts[i].net_names[j-2][0]);
		}
	}
}




////////// Module name Print //////////
void Sampleparser::Module_Print()
{
	cout << endl;
	cout << "module ";
	for (int i = 0; i < origin_module.size(); i++)
	{
		if (i == 1)
			cout << "(";
		cout << origin_module[i];
		if (i != 0)
		{
			cout << ", ";
		}
	}
	cout << "\b\b";
	cout << ");" << " ";

	cout << endl;
}

////////// Input name Print //////////
void Sampleparser::Input_Print()
{
	cout << "input" << " ";
	for (int i = 0; i < inputs.size(); i++)
		cout << inputs[i] << ", ";
	cout << "\b\b";
	cout << ";" << endl;
}

////////// Output name Print //////////
void Sampleparser::Output_Print()
{
	cout << "output " << output << ";"<<endl;
	cout << endl;
}

////////// Wire name Print //////////
void Sampleparser::Wire_Print()
{
	for (size_t i = 0; i < nets.size(); i++)
	{
		for (size_t n = 0; n < nets[i].names.size(); n++)
			cout << "wire " << nets[i].names[n] << ";" << endl;
	}
	ADD_WIRES();
	for(size_t i = 0; i<add_wires.size();i++)
		cout << "wire " << add_wires[i] << ";" << endl;
	cout << endl;
}


////////// Instance name Print //////////
void Sampleparser::Instance_Print()
{
	Origin_Insts();
	INSERTION_INSTS();
	for (int i = 0; i < ORIGIN_INSTS.size(); i++)
	{
		for (int j = 0; j < ORIGIN_INSTS[i].size(); j++)
		{
			/*if (ORIGIN_INSTS[i][j].empty())
				break;*/

			if (j == 2)
			{
				cout << "(";
			}
			cout << ORIGIN_INSTS[i][j] << " ";

			if (j != 0 && j != 1)
			{
				cout << "\b";
				cout << ", ";
			}	
		}
		cout << "\b\b";
		cout << "); ";
		cout << endl;
	}
	
	int x, y;

	for (x = before_NUM_BUFFER; x < NUM_BUFFER; x++)
	{
		if (x != 0)
			cout << ");" << endl;
		for (y = 0; y < 4; y++)
		{
			if (ADD_INSTS[x][y].empty())
				break;

			if (y == 2)
			{
				cout << "(";
			}
			cout << ADD_INSTS[x][y] << " ";

			if (y != 0 && y != 1)
			{
				cout << "\b";
				cout << ", ";
			}
		}
		cout << "\b\b";
	}

	cout << endl;
	cout << "endmodule" << endl;
}

void Sampleparser::PRINT()
{
	Module_Print();
	Input_Print();
	Output_Print();
	Wire_Print();
	Instance_Print();
}


//	Verilog File Print
void Sampleparser::FILE_PRINT()
{
	ofstream fout("CTS_sample.v", std::ios::out);
	if (fout.is_open())
	{
		cout << "파일 열기 성공!";
		//	module
		fout << "module ";
		for (int i = 0; i < origin_module.size(); i++)
		{
			if (i == 1)
				fout << "(";
			fout << origin_module[i];
			if (i != 0 && i != origin_module.size() - 1)
			{
				fout << ", ";
			}
		}
		fout << ");" << " ";
		fout << endl;
		//	input 
		fout << "input" << " ";
		for (int i = 0; i < inputs.size(); i++)
		{
			fout << inputs[i];
			if (i != inputs.size() - 1)
				fout << ", ";
		}
		fout << ";" << endl;
		//	output
		fout << "output " << output << ";" << endl;
		fout << endl;
		//	wire
		for (size_t i = 0; i < nets.size(); i++)
		{
			for (size_t n = 0; n < nets[i].names.size(); n++)
				fout << "wire " << nets[i].names[n] << ";" << endl;
		}
		ADD_WIRES();
		for (size_t i = 0; i < add_wires.size(); i++)
			fout << "wire " << add_wires[i] << ";" << endl;
		fout << endl;
		//	instance
		for (int i = 0; i < ORIGIN_INSTS.size(); i++)
		{
			for (int j = 0; j < ORIGIN_INSTS[i].size(); j++)
			{
				if (j == 2)
				{
					fout << "(";
				}
				fout << ORIGIN_INSTS[i][j] << " ";

				if (j != 0 && j != 1 && j != ORIGIN_INSTS[i].size() - 1)
				{
					fout << ", ";
				}
			}
			fout << "); " << endl;
		}

		int x, y;
		for (x = before_NUM_BUFFER; x < NUM_BUFFER; x++)
		{
			if (x != 0)
				fout << ");" << endl;
			for (y = 0; y < 4; y++)
			{
				if (ADD_INSTS[x][y].empty())
					break;

				if (y == 2)
				{
					fout << "(";
				}
				fout << ADD_INSTS[x][y] << " ";

				if (y != 0 && y != 1 && y != 3)
				{
					fout << ", ";
				}
			}
		}
		
		fout << endl;
		fout << "endmodule" << endl;
	}
	
	fout.close();
}



void Sampleparser::Visualization()
{
	//	CTS 진행하기 전
	FILE* f1 = NULL;
	fopen_s(&f1, "graph.dot", "wt");
	vector<int> node_num;
	fprintf(f1, "digraph G \n{\n");
	Graph_Visualization(getoutput(), node_num, f1);
	fprintf(f1, "\n}");
	fclose(f1);


	//	CTS 진행한 후
	FILE* f2 = NULL;
	fopen_s(&f2, "CTS_graph.dot", "wt");
	vector<string> strname;
	vector<int> node_num2;
	fprintf(f2, "digraph G \n{\n");
	CTS_Graph_Visualization(getoutput(),strname, node_num2, f2);
	fprintf(f2, "\n}");
	fclose(f2);

}


//	
int NODE_COUNT = 0;
void Sampleparser::Graph_Visualization(string name, vector<int> node_num, FILE*f1)
{
	if (isinput(name))
	{
		for (int i = 0; i < node_num.size(); i++)
		{
			fprintf(f1, "%d", node_num[i]);
			cout << node_num[i];
			if (i != node_num.size() - 1)
			{
				cout << "->";
				fprintf(f1, "->");
			}
		}
		cout << endl;
		fprintf(f1, "\n");
		return;
	}
	NODE_COUNT = NODE_COUNT + 1;
	cout << (int)NODE_COUNT << "[label = " << "\"" << Gatedelay(getmodule(name)) << "\"]" << endl;
	fprintf(f1, "%d [label =  %d]\n", (int)NODE_COUNT, Gatedelay(getmodule(name)));
	node_num.push_back(NODE_COUNT);
	
	vector<string> children = getchildren(name);
	

	bool allchild = true;	// child가 모두 input일 경우
	for (int i = 0; i < children.size(); i++)
	{
		if (!isinput(children[i]))
		{
			allchild = false;
			break;
		}
	}

	for (int i = 0; i < children.size(); i++)
	{
		Graph_Visualization(children[i],node_num, f1);
		if (allchild && isinput(children[i]))
			break;
	}
	return;
}


int NODE_COUNT1 = 0;
void Sampleparser::CTS_Graph_Visualization(string name, vector<string> strname,vector<int> node_num2, FILE* f2)
{
	int bufferdelay = 0;
	int buffer_num=0;
	vector<string> children;
	if (isinput(name))
	{
		int total = 0;
		int i;

		for (i = 0; i < strname.size(); i++)
			total += Gatedelay(getmodule(strname[i]));
		
		string tmp = strname[i - 1];
		if (total < MaxDelay())
		{
			int diff = MaxDelay() - total;
			bufferdelay = Bufferinsertion(tmp, diff);
			buffer_num = (diff / bufferdelay);
			NODE_COUNT1 += buffer_num;
		}

		for (int i = 0; i < buffer_num; i++)
		{
			NODE_COUNT1++;
			node_num2.push_back(NODE_COUNT1);
			cout << (int)NODE_COUNT1 << "[label = " << "\"" << bufferdelay << "\"]" << endl;
			fprintf(f2, "%d [label =  %d]\n", (int)NODE_COUNT1, bufferdelay);
		}

		for (int i = 0; i < node_num2.size(); i++)
		{
			fprintf(f2, "%d", node_num2[i]);
			cout << node_num2[i];
			if (i != node_num2.size() - 1)
			{
				cout << "->";
				fprintf(f2, "->");
			}
		}

		cout << endl;
		fprintf(f2, "\n");
		return;

		return;
	}

	strname.push_back(name);
	NODE_COUNT1 = NODE_COUNT1 + 1;
	cout << (int)NODE_COUNT1 << "[label = " << "\"" << Gatedelay(getmodule(name)) << "\"]" << endl;
	fprintf(f2, "%d [label =  %d]\n", (int)NODE_COUNT1, Gatedelay(getmodule(name)));
	node_num2.push_back(NODE_COUNT1);

	children = getchildren(name);

	bool allchild = true;	// child가 모두 input일 경우
	for (int i = 0; i < children.size(); i++)
	{
		if (!isinput(children[i]))
		{
			allchild = false;
			break;
		}
	}

	for (int i = 0; i < children.size(); i++)
	{
		CTS_Graph_Visualization(children[i], strname, node_num2, f2);
		if (allchild && isinput(children[i]))
			break;
	}

	return;
}





void Sampleparser::add_module(std::string&& name) {
	if (module_cnt > 0)
	{
		print();
		
		ports.clear();
		nets.clear();
		insts.clear();
		inputs.clear();
		output = "";
		origin_module.clear();
		ADD_INSTS.clear();
		ORIGIN_INSTS.clear();
		add_wires.clear();
		INSERT_BUFFER.clear();
	}
		
	std::cout << "Module name = " << name << endl;
	origin_module.push_back(name);
	module_cnt++;
}

void Sampleparser::add_port(verilog::Port&& port) {
	std::cout << "Port: " << port << endl;
	if (port.dir == PortDirection::INPUT)
	{
		for (size_t n = 0; n < port.names.size(); n++)
		{
			inputs.push_back(port.names[n]);
			origin_module.push_back(port.names[n]);
		}
	}
	if (port.dir == PortDirection::OUTPUT)
	{
			output = port.names[0];
			origin_module.push_back(output);
	}

	ports.push_back(std::move(port));
}

void Sampleparser::add_net(verilog::Net&& net) {
	std::cout << "Net: " << net << endl;
	nets.push_back(std::move(net));
}

void Sampleparser::add_assignment(verilog::Assignment&& ast) {
	std::cout << "Assignment: " << ast << endl;
	assignments.push_back(std::move(ast));

}

void Sampleparser::add_instance(verilog::Instance&& inst) {
	std::cout << "Instance: " << inst << '\n';
	insts.push_back(std::move(inst));
}


void Sampleparser::print()
{
	////////////////// ports
	for (size_t i = 0; i < ports.size(); i++)
	{
		for (size_t n = 0; n < ports[i].names.size(); n++)
			cout << "port:" << ports[i].dir << " " << ports[i].names[n] << endl;
	}

	////////////////// nets
	for (size_t i = 0; i < nets.size(); i++)
	{
		for (size_t n = 0; n < nets[i].names.size(); n++)
			cout << "nets:" << nets[i].type << " " << nets[i].names[n] << endl;
	}

	////////////////// inst
	for (size_t i = 0; i < insts.size(); i++)
	{
		cout << "-----------------" << endl;
		//cout << insts[i] << endl;
		cout << "module : " << insts[i].module_name << endl;
		cout << "inst : " << insts[i].inst_name << endl;

		for (size_t n = 0; n < insts[i].net_names.size(); n++) {
			cout << "net : " << std::get<std::string>(insts[i].net_names[n][0]) << endl;
		}

	}

	traversal();
	cout << endl;

	int maxdelay = MaxDelay();
	cout << "maxdelay:" << maxdelay << endl;
	cout << endl;
	route();
	cout << endl;
	addBuffer();
	PRINT();
	FILE_PRINT();
	// before_NUM_BUFFER = NUM_BUFFER;
}

  