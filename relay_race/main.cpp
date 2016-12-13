#define MAINFILE

#include "race.h"
#include <unistd.h>
#include <QCoreApplication>


int main(int argc, char **argv)
{
	QCoreApplication a(argc, argv);

	MyObject ob[THREAD_AMOUNT];
	Thread1 leadth(0);	
	Thread2 threads[THREAD_AMOUNT - 1];

	for (int i = 0; i < THREAD_AMOUNT - 1; i++)
	{
		threads[i].setid(i + 1);
	}

	QObject::connect(&leadth, SIGNAL(pass_id(int)), &(ob[0]), SLOT(print_id(int)));	
	
	for (int i = 0; i < THREAD_AMOUNT - 1; i++)
	{
		QObject::connect(&(threads[i]), SIGNAL(pass_id(int)), &ob[i + 1], SLOT(print_id(int)));		
	}

	for (int i = 0; i < THREAD_AMOUNT - 1; i++)
	{
		threads[i].start();	
		ob[i].moveToThread(&(threads[i]));	
	}

	leadth.start();
	leadth.wait();

	for (int i = 0; i < THREAD_AMOUNT - 1; i++)
	{
		threads[i].quit();
		threads[i].wait();		
	}

	return 0;

}