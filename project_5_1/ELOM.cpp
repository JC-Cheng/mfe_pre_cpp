#include "ELOM.h"
#include <cmath>
#include <iostream>
//Assignment 5: ELOM.cpp
#include <fstream>
#include <string>
using namespace std;

int MatchOrders(Order *B, Order *S);

//Order sorting help-function
bool  Buy_OrderSort(Order* lhs, Order* rhs)
{
	if (lhs->getTicker() == rhs->getTicker()) return lhs->get_price() > rhs->get_price();
	else return lhs->getTicker() < rhs->getTicker();
}
bool  Sell_OrderSort(Order* lhs, Order* rhs)
{
	if (lhs->getTicker() == rhs->getTicker()) return lhs->get_price() < rhs->get_price();
	else return lhs->getTicker() < rhs->getTicker();
}

//*************
//*** ORDER ***
//*************

Order::Order()
{
}

Order::~Order()
{
}

Order::Order(string NAME,int ID,int No_Shares,double Price) : m_ticker(NAME), m_id(ID), m_no_shares(No_Shares), m_price(Price) 
{
	
}

string Order::getTicker()
{
	return m_ticker;
}

void Order::insertTicker(string NAME)
{
	m_ticker = NAME;
}

int Order::get_id()
{
	return m_id;
}

int Order::get_no_shares()
{
	return m_no_shares;
}

double Order::get_price()
{
	return m_price;
}

void Order::insertID(int ID) {
	m_id= ID;
}

void Order:: insertNoShares(int NO_SHARES)
{ 
	m_no_shares=NO_SHARES;
}

void Order::insertPrice(double PRICE) 
{
	 m_price=PRICE;
}

//************
//*** ELOM ***
//************

ELOM::ELOM()
{
}

ELOM::~ELOM()
{
}

void ELOM::addOrder(Order * order, list<Order*>* listOfOrders)
{
	(*listOfOrders).push_back(order);
}

int ELOM::countOrders(list<Order*> * listOfOrders)
{
	return (*listOfOrders).size();
}

int ELOM::countTotalOrders()
{
	return this->countOrders(&m_BUY) + this->countOrders(&m_SELL);
}


// Function to read from lists of orders (BUY and SELL)
void ELOM::readListOfOrders(string fileBuy,string fileSell)
{
	ifstream fb(fileBuy.c_str());
	ifstream fs(fileSell.c_str());

	if(fb && fs) //No need to open or close the file, it is done anyways. Verify if the streams are OK.
	{

		string line;

		//BUY
		while(getline(fb,line))
		{
			Order * order = new Order(); //DO NOT delete this pointer, because it represents an element of the BUY list and we work with it therafter
			if (parseLine(line, order)) //Use of the routine parseLine
			{ 
				// We parse the line currently read and  add an order to the market outstanding orders
				this->addOrder(order, &m_BUY);
				
			}
			else
			{
				cout << "error reading line: '" << line << "'" << endl;
			}
		}
		
		//SELL
		while(getline(fs,line))
		{
			Order * order = new Order(); //DO NOT delete this pointer, because it represents an element of the BUY list and we work with it therafter
			if (parseLine(line, order))
			{ 
				// We parse the line currently read and  add an order to the market outstanding orders
				this->addOrder(order, &m_SELL);
			}
			else
			{
				cout << "error reading line: '" << line << "'" << endl;
			}
		}
	}
	else
	{
		cout << "Bad file name. Execution aborted." << endl;
	}
}

// Function to output updated lists of orders (BUY and SELL)
void ELOM::writeListOfOrders(string fileBuy, string fileSell)
{
	ofstream fb(fileBuy.c_str());
	ofstream fs(fileSell.c_str());

	if(fb && fs) //No need to open or close the file, it is done anyways. Verify if the streams are OK.
	{
		//BUY
		//fb << "Ticker, TraderID, NumberOfShares, Price" << endl;   //Uncomment if header of file wanted
		for(list<Order*>::iterator it = m_BUY.begin(); it != m_BUY.end(); it++)
		{
			fb << (*it)->getTicker() << " " << (*it)->get_id() << " " << (*it)->get_no_shares() << " " << (*it)->get_price() << endl;
		}

		//SELL
		//fs << "Ticker, TraderID, NumberOfShares, Price" << endl;  //Uncomment if header of file wanted
		for(list<Order*>::iterator it = m_SELL.begin(); it != m_SELL.end(); it++)
		{
			fs << (*it)->getTicker() << " " << (*it)->get_id() << " " << (*it)->get_no_shares() << " " << (*it)->get_price() << endl;
		}

	}
	else
	{
		cout << "Bad file name in writeListOfOrders. Execution aborted." << endl;
	}

}


//************************
//*** Helper functions ***
//************************

bool nextToken(const string& line, int& offset, string& token)
{
	int len = line.length();
	bool rc = (offset < len);
	token = "";
	bool done = false;
	while (!done && (offset < len))
	{
		char c = line.at(offset);
		if (c == ',' || c == '\r' || c == '\n' || c == '\t' || c == ' ' )
		{
			done = true;
		} else {
			token += c;
		}
		offset++;
	}
	return rc;
}


bool parseLine(const string& line,Order* order)
{
	int offset = 0;
	string token;

	bool rc = nextToken(line, offset, token);
	order->insertTicker(token.c_str());
	rc &= nextToken(line, offset, token);
	order->m_id = atof(token.c_str());
	rc &= nextToken(line, offset, token); //atof->int->string
	order->m_no_shares = atof(token.c_str());
	rc &= nextToken(line, offset, token);
	order->m_price = atof(token.c_str());
	
	return rc;
}


int ELOM::Execute()
{
	// THIS IS THE METHOD YOU ARE ASKED TO IMPLEMENT IN ASSIGNMENT 5
	
	//Sort order by stock name, and then by limit price
	m_BUY.sort(Buy_OrderSort);  //buy orders are sorted in descending limit prices
	m_SELL.sort(Sell_OrderSort); //sell orders are sorted in ascending limit prices

	int Total_Vol = 0;
	list<Order*>::iterator Bit = m_BUY.begin(), Sit = m_SELL.begin();
	
	while (Bit != m_BUY.end() && Sit != m_SELL.end())
	{
		// same ticker
		if ((*Bit)->getTicker() == (*Sit)->getTicker())
		{
			if ((*Bit)->get_price() < (*Sit)->get_price()) { Bit++; Sit++; } //no dealing
			else {
				//match and update orders
				Total_Vol += MatchOrders(*Bit, *Sit);	
				//clear executed orders and move iterator
				if ((*Bit)->get_no_shares() == 0) Bit = m_BUY.erase(Bit);
				if ((*Sit)->get_no_shares() == 0) Sit = m_SELL.erase(Sit);
			}
		}
		// different tickers, move to next order
		else if ((*Bit)->getTicker() < (*Sit)->getTicker()) Bit++;
		else Sit++;
	}

	return Total_Vol;
}

int MatchOrders(Order *B, Order *S)
{
	int Vol = 0;
	//same ticker and transaction took place
	if (B->get_price() >= S->get_price() && B->getTicker() == S->getTicker()) {
		if (B->get_no_shares() > S->get_no_shares()) {
			//# of demand > # of supply, count trading volumne and update orders
			Vol = S->get_no_shares();
			S->insertNoShares(0);
			B->insertNoShares(B->get_no_shares() - Vol);
		}
		else {
			//# of demand <= # of supply, count trading volumne and update orders
			Vol = B->get_no_shares();
			B->insertNoShares(0);
			S->insertNoShares(S->get_no_shares() - Vol);
		}
	}
	//for debug purpose
	//if (Vol) cout << B->getTicker() << " " << B->get_price() << " " << S->get_price() << " " << Vol << endl;
	return Vol;
}