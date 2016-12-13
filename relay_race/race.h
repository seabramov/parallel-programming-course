#ifndef RACE_H
#define RACE_H

#ifdef MAINFILE
    #define EXTERN
#else
    #define EXTERN extern
#endif

#include <QThread>
#include <iostream>
#include <qmutex.h>

using namespace std;

#define THREAD_AMOUNT	20

class MyObject : public QObject
{
	Q_OBJECT
public slots:
	void print_id(int i)
	{
		cout << "Slot called" << endl;
		cout << "Hello! I was called by " << i << " thread" << endl;
	}
};

class Thread1 : public QThread
{
	Q_OBJECT
public:
		
	Thread1(int i = 0) { thid = i;}
	void setid(int i)	{ thid = i;}
	
	void run()
	{
		cout << "Leader started" << endl;
		sleep(1);
		emit pass_id(thid);
	}

signals:
	void pass_id(int id);

private:
	int thid;

};


class Thread2 : public QThread
{
	Q_OBJECT
public:

	Thread2(int i = 0) { thid = i;}
	void setid(int i)	{ thid = i;}

	void run()
	{
		cout << "Worker started" << endl;
		sleep(1);
		exec();
		cout << "My thread id: " << thid << endl << endl;;
		sleep(1);
		emit pass_id(thid);
	}

signals:
	void pass_id(int id);

private:
	int thid;
};

#endif // RACE_H