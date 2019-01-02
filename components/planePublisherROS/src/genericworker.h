/*
 *    Copyright (C)2019 by YOUR NAME HERE
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef GENERICWORKER_H
#define GENERICWORKER_H

#include "config.h"
#include <stdint.h>
#include <qlog/qlog.h>

#if Qt5_FOUND
	#include <QtWidgets>
#else
	#include <QtGui>
#endif
#include <ui_mainUI.h>

#include <CommonBehavior.h>

#include <ros/ros.h>
#include <RoboCompPlaneROS/PointXY.h>
#include <RoboCompPlaneROS/Dimensions.h>
#include <RoboCompPlaneROS/Points.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>

#define CHECK_PERIOD 5000
#define BASIC_PERIOD 100

using namespace std;

typedef map <string,::IceProxy::Ice::Object*> MapPrx;


	//class for rosPublisher
class PublisherPlane
{
public:
	ros::Publisher pub_newDimensions;
	ros::Publisher pub_newPlaneName;
	PublisherPlane(ros::NodeHandle *node)
	{
		pub_newDimensions = node->advertise<RoboCompPlaneROS::Dimensions>(node->resolveName("newDimensions"), 1000);
		pub_newPlaneName = node->advertise<std_msgs::String>(node->resolveName("newPlaneName"), 1000);
	}
	~PublisherPlane(){}
	void newDimensions(RoboCompPlaneROS::Dimensions dims)
	{
		pub_newDimensions.publish(dims);
	}
	void newPlaneName(std_msgs::String planeName)
	{
		pub_newPlaneName.publish(planeName);
	}
};


class GenericWorker :
#ifdef USE_QTGUI
public QWidget, public Ui_guiDlg
#else
public QObject
#endif
{
Q_OBJECT
public:
	GenericWorker(MapPrx& mprx);
	virtual ~GenericWorker();
	virtual void killYourSelf();
	virtual void setPeriod(int p);

	virtual bool setParams(RoboCompCommonBehavior::ParameterList params) = 0;
	QMutex *mutex;




protected:
	QTimer timer;
	int Period;
	ros::NodeHandle node;
	PublisherPlane *plane_proxy;

private:


public slots:
	virtual void compute() = 0;
	virtual void initialize(int period) = 0;
signals:
	void kill();
};

#endif