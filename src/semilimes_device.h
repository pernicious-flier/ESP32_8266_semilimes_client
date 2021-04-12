#ifndef semilimes_talk_h
#define semilimes_talk_h

#include <Arduino.h>
#include "semilimes.h"

class semilimes_device : public semilimes
{
	public:
		String dev_AuthToken;
		String dev_ReceiverId;
		String dev_name;
		int	dev_id;
		int dev_group;
		int dev_subgroup;
		int dev_n_out;
		int dev_n_in;
		int dev_outputs[10];
		int dev_inputs[10];
		void init(String AuthToken, String name, int id, int group,	int subgroup, int n_out, int n_in);
		String parse_msg(String msg, String ReceiverId);
		virtual int SetOutSpecific(int output_n,int value);
		virtual int GetInSpecific(int input_n);
		
	private:
		String SetOut(String name, int id, int group,	int subgroup, int output_n, int value);
		String GetIn(String name, int id, int group,	int subgroup, int input_n);
		String help(void);
};

#endif