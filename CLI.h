/*
 * CLI.H
 *
 * Author: 207233222 Yeheli Frangi
 */

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
	vector<Command*> commands;
protected:
	struct Data{
		TimeSeries learn_normal;
		TimeSeries detect;
		AnomalyReport report;
	};

public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
};

#endif /* CLI_H_ */
