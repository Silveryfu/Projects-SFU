#include "header.h"

using namespace std;

int main() {
	int pip[SLAVES_NUMBER][2];
	for (int i=0; i<SLAVES_NUMBER; i++) pipe(pip[i]);

	return 0;
}
