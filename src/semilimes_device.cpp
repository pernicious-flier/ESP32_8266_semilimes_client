#include "semilimes_device.h"

void semilimes_device::init(String AuthToken, String name,	int	id,	int group,	int subgroup, int n_out, int n_in)
{
	dev_AuthToken = AuthToken;
	dev_name = name;
	dev_id = id;
	dev_group = group;
	dev_subgroup = subgroup;
	dev_n_out = n_out;
	dev_n_in = n_in;
}

int semilimes_device::SetOutSpecific(int output_n,int value){};

int semilimes_device::GetInSpecific(int input_n){};
  
String semilimes_device::SetOut(String name, int id, int group,	int subgroup, int output_n, int value)
{
	if(output_n<=dev_n_out)
	{
		if(((name==dev_name or name=="" ) and id==0 and group==0 and subgroup==0)
		or ((name==dev_name or name=="" ) and id==dev_id and group==0 and subgroup==0) 
		or ((name==dev_name or name=="" ) and id==dev_id and group==dev_group and subgroup==0) 
		or ((name==dev_name or name=="" ) and id==dev_id and group==dev_group and subgroup==dev_subgroup))
		{
			dev_outputs[output_n] = SetOutSpecific(output_n, value);
			
			//String Body = "&> n:"+dev_name+" i:"+dev_id+" g:"+dev_group+" :s"+dev_subgroup+" :p"+output_n+" v:"+dev_outputs[output_n];
			//return SendTxtMsg(dev_AuthToken, dev_ReceiverId, semilimes_channel, Body);
			String msg = "<html> \
			<body> \
			<h1>Value Set:   ";
			msg += String(dev_outputs[output_n])+"<\h1>\
			<h2>device Name: ";
			msg += dev_name+"</h2>\
			<h3>ouput setting msg:</h3>\
			<p>Device ID: ";
			msg += String(dev_id)+"</p>\
			<p>Group ID: ";
			msg += String(dev_group)+"</p>\
			<p>Subgroup ID: ";
			msg += String(dev_subgroup)+"</p>\
			<p>Ouput Number: ";
			msg += String(output_n)+"</p>\
			</body> \
			</html>";
			return SendHTML(dev_AuthToken, dev_ReceiverId, semilimes_channel, msg);
		}
		return "none";			
	}
	return "none";
}

String semilimes_device::GetIn(String name, int id, int group, int subgroup, int input_n)
{
	if(input_n<=dev_n_in)
	{
		if(((name==dev_name or name=="" ) and id==0 and group==0 and subgroup==0)
		or ((name==dev_name or name=="" ) and id==dev_id and group==0 and subgroup==0) 
		or ((name==dev_name or name=="" ) and id==dev_id and group==dev_group and subgroup==0) 
		or ((name==dev_name or name=="" ) and id==dev_id and group==dev_group and subgroup==dev_subgroup))
		{
			dev_inputs[input_n] = GetInSpecific(input_n);

			//String Body = "&< n:"+dev_name+" i:"+dev_id+" g:"+dev_group+" s:"+dev_subgroup+" p:"+input_n+" v:"+dev_inputs[input_n];
			//return SendTxtMsg(dev_AuthToken, dev_ReceiverId, semilimes_channel, Body);
			String msg = "<html> \
			<body> \
			<h1>Value read:   ";
			msg += String(dev_inputs[input_n])+"<\h1>\
			<h2>device Name: ";
			msg += dev_name+"</h2>\
			<h3>input reading msg:</h3>\
			<p>Device ID: ";
			msg += String(dev_id)+"</p>\
			<p>Group ID: ";
			msg += String(dev_group)+"</p>\
			<p>Subgroup ID: ";
			msg += String(dev_subgroup)+"</p>\
			<p>Input Number: ";
			msg += String(input_n)+"</p>\
			</body> \
			</html>";
			return SendHTML(dev_AuthToken, dev_ReceiverId, semilimes_channel, msg);
		}
		return "none";
	}
	else if(input_n==dev_n_in+1)
	{
		GetInSpecific(input_n);
		return "none";
	}
	return "none";
}

String semilimes_device::help()
{
	String msg = "<html> \
    <body> \
    <h2>semilimes machine2machine protocol</h2>\
	<h3>message structure</h3>\
	<p>start dir n(name). i(id). g(group). s(subg). p(port). v(val)\
	<h3>parameters</h3>\
	<p><u>start</u>: start char = #</p>\
	<p><u>dir</u>: direction, < (to device) , > (from device)</p>\
	<p><u>name</u>: device name(String)</p>\
	<p><u>id</u>: device ID (int)</p>\
	<p><u>group</u>: device group (int 1..9)</p>\
	<p><u>subg</u>: device sub group (int 1..9)</p>\
	<p><u>port</u>: device I/O port number (int 1..9)</p>\
	<p><u>val</u>: output value(int)</p>\
	<h3>info</h3>\
	<p>You don't have to use all parameters, just what you need.</p>\
	<p>If you set 'i', 'g' or 's' to '0', you are selecting all ip, all group, all subgroup</p>\
	<p>If you dont't set 'i', 'g', 's' in the msg, are set to '0' by default</p>\
	<p>sample message to set '1' to Port1 digital output, of the device called 'Device1', id=1, group=1, subgroup=1:</p>\
	<p># > nO_Device1. i1. g1. s1. p0. v1.</p>\
	<p>sample message to get a digital input Port1, from a device with id=1, group=2, subgroup=3:</p>\
	<p># < i1. g2. s3. p1.</p>\
	<p>sample message to get inputs from all devs in group 1 port 1:</p>\
	<p># < g1. p1.</p>\
	<p>message to get this guide: #h</p>\
    </body> \
    </html>";
	//return SendTxtMsg(dev_AuthToken, dev_ReceiverId, semilimes_channel, msg);
    return SendHTML(dev_AuthToken, dev_ReceiverId, semilimes_channel, msg);
}

//# start char
//<>(> from device to ch, < from ch to device) 
//n(device name, ends with ',') 
//i(id device) 
//g(device group)
//s(device subgroup)
//p(n i/o port)
//v(value, ends with ',')
//id,group and subgroup == 0 mean "all"
String semilimes_device::parse_msg(String msg, String ReceiverId)
{
	int i=0;
	char direction,option;//>=out, <=in
	String name,tmpVal,result;//ends with ','
	int id=0,group=0,subgroup=0,port=0,value=0;
	
	dev_ReceiverId = ReceiverId;
	
	if(msg[i]=='#')
	{
		i++;
		while(i<msg.length())
		{			
			if(msg.charAt(i)=='h' and dev_id==1)//help
			{
				option = msg.charAt(i);
			}			
			if(msg.charAt(i)=='>' or msg.charAt(i)=='<')//>output , <input
			{
				direction = msg.charAt(i);
				i++;
			}
			if(msg.charAt(i)=='n')//name
			{
				int j=0;
				i++;
				while(j<(msg.length()-i) and msg.charAt(i+j) != '.')
				{
					name += msg.charAt(i+j);
					j++;
				}
				if(msg[i+j] != '.') 
				{
					name = "";
				}
				i=i+j;
			}
			if (msg.charAt(i)=='i')//id device
			{
				i++;
				id = (int)msg.charAt(i)-48;
			}
			if (msg.charAt(i)=='g')//group id
			{
				i++;
				group = (int)msg.charAt(i)-48;
			}
			if (msg.charAt(i)=='s')//subgroup id
			{
				i++;
				subgroup = (int)msg.charAt(i)-48;
			}
			if (msg.charAt(i)=='p')//i/o port
			{
				i++;
				port = (int)msg.charAt(i)-48;
			}
			if(msg.charAt(i)=='v')//value
			{
				int j=0;
				i++;
				while(j<(msg.length()-i) and msg.charAt(i+j) != '.')
				{
					tmpVal += msg.charAt(i+j);
					j++;
				}
				if(msg[i+j] != '.') { value = 0; }
				else 
				{ 					
					for(int q=0;q<j;q++)
					{
						value += ((int)tmpVal.charAt(q)-48)*pow(10.0,(float)(j-1-q));
					}
				}
				i=i+j; 
				
			}
			i++;
		}
		if(direction == '<')
		{
			result = this->GetIn(name, id, group, subgroup, port);
		}
		else if(direction == '>') 
		{
			result = this->SetOut(name, id, group, subgroup, port, value);
		}
		else if(option == 'h')
		{
			result = this->help();
		}
	}
	else return "none";
	

	return result;
}