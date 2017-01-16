////Assignment 5: Main.cpp
#include <iostream>
#include <fstream>
#include <string>
#include "ELOM.h"

using namespace std;

int main(void) {

	ELOM * myElom = new ELOM();
	
	//Read a file of BUY orders and a file of SELL orders
	myElom->readListOfOrders("ORDERS_BUY.txt","ORDERS_SELL.txt");

	//Match and clear trades from the market
	cout << "Before execution, ELOM received " << myElom->countTotalOrders() << " orders." << endl;
	cout << myElom->Execute() << " shares have been traded." << endl;
	cout << "After execution, there are " << myElom->countTotalOrders() << " orders remaining in the ELOM." << endl;

	//Write the updated lists of BUY orders and SELL orders after we have cleared the market
	myElom->writeListOfOrders("ORDERS_BUY_OUTPUT.TXT", "ORDERS_SELL_OUTPUT.TXT");

	system("PAUSE"); //For windows environment
	return 0;
}