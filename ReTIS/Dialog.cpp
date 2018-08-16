
#include "Main.h"

void	cDialog::Update() {
	if (end - start > wait_time) {
		out += message[str_count];
		str_count++;
		start = end;
	}
	end = clock();
}

void	cDialog::Render() {

}