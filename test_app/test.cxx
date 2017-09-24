#include "test.h"
#include "directX_renderer.h"
#include <iostream>

using namespace std;


int main() {
	auto const b = test_init();
	cout << "test return from dll = " << b << endl;
}