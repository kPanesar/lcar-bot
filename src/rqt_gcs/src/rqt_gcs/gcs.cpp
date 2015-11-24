#include <rqt_gcs/gcs.h>
#include <pluginlib/class_list_macros.h>
#include <QStringList>
#include <iomanip>
namespace rqt_gcs {

MyPlugin::MyPlugin()
  : rqt_gui_cpp::Plugin()
  , widget_(0)
{
	
  // Constructor is called first before initPlugin function, needless to say.
  // give QObjects reasonable names
  setObjectName("MyPlugin");
}

void MyPlugin::initPlugin(qt_gui_cpp::PluginContext& context)
{
  // access standalone command line arguments
  QStringList argv = context.argv();
  // create QWidget
  widget_ = new QWidget();
  missionCancelWidget_ = new QWidget();
  missionSelectWidget_ = new QWidget();
  missionProgressWidget1_ = new QWidget();
  missionProgressWidget2_ = new QWidget();
  missionProgressWidget3_ = new QWidget();
  missionProgressWidget4_ = new QWidget();
  UavQuestionWidget_ = new QWidget();
  UavStatWidget1_ = new QWidget();
  UavStatWidget2_ = new QWidget();


  // extend the widget with all attributes and children from UI file
  ui_.setupUi(widget_);
  mcUi_.setupUi(missionCancelWidget_);

  mpUi1_.setupUi(missionProgressWidget1_);
  mpUi2_.setupUi(missionProgressWidget2_);
  mpUi3_.setupUi(missionProgressWidget3_);
  mpUi4_.setupUi(missionProgressWidget4_);

  msUi_.setupUi(missionSelectWidget_);
  uqUi_.setupUi(UavQuestionWidget_);

  usUi1_.setupUi(UavStatWidget1_);
  usUi2_.setupUi(UavStatWidget2_);

  
  // add widget to the user interface
  context.addWidget(widget_);
  ui_.MissionProgressIndividualLayout->addWidget(missionProgressWidget1_);
  ui_.MissionProgressIndividualLayout->addWidget(missionProgressWidget2_);
  ui_.MissionProgressIndividualLayout->addWidget(missionProgressWidget3_);
  ui_.MissionProgressIndividualLayout->addWidget(missionProgressWidget4_);
  ui_.UavStatLayout->addWidget(UavStatWidget1_);
  ui_.UavStatLayout->addWidget(UavStatWidget2_);
 

   updateTimer = new QTimer(this);
   connect(updateTimer, SIGNAL(timeout()), this, SLOT(TimedUpdate()));
   updateTimer->start(100); 
}

void MyPlugin::Calculate(){

}


void MyPlugin::TimedUpdate(){
     
    tempData = quadControl.GetState().mode.c_str();
    usUi1_.flightModeDisplay->setText(tempData);

    tempData.setNum(quadControl.GetFlightState().yaw);
    usUi1_.yawDisplay->setText(tempData);

    tempData.setNum(quadControl.GetFlightState().roll);
    usUi1_.rollDisplay->setText(tempData);

    tempData.setNum(quadControl.GetFlightState().pitch);
    usUi1_.pitchDisplay->setText(tempData);

    tempData.setNum(quadControl.GetFlightState().altitude);
    usUi1_.altitudeDisplay->setText(tempData);

    tempData.setNum(quadControl.GetFlightState().vertical_speed);
    usUi1_.verticalSpaceDisplay->setText(tempData);

    tempData.setNum(quadControl.GetFlightState().ground_speed);
    usUi1_.horizontalSpaceDisplay->setText(tempData);

    tempData.setNum(quadControl.GetFlightState().heading);
    usUi1_.headingDisplay->setText(tempData);

   
    usUi1_.waypointDisplay->setText("89");

    tempData.setNum(quadControl.GetBatteryStatus().remaining*100);
    //tempData.setNum(.99*100);
    usUi1_.batteryProgressBar->setValue(tempData.toInt());

}




void MyPlugin::shutdownPlugin()
{
  // TODO unregister all publishers here =
 
}

void MyPlugin::saveSettings(qt_gui_cpp::Settings& plugin_settings, qt_gui_cpp::Settings& instance_settings) const
{
  // TODO save intrinsic configuration, usually using:
  // instance_settings.setValue(k, v)
}

void MyPlugin::restoreSettings(const qt_gui_cpp::Settings& plugin_settings, const qt_gui_cpp::Settings& instance_settings)
{
  // TODO restore intrinsic configuration, usually using:
  // v = instance_settings.value(k)
}

/*bool hasConfiguration() const
{
  return true;
}

void triggerConfiguration()
{
  // Usually used to open a dialog to offer the user a set of configuration
}*/

} // namespace
PLUGINLIB_DECLARE_CLASS(rqt_gcs, MyPlugin, rqt_gcs::MyPlugin, rqt_gui_cpp::Plugin)
//PLUGINLIB_EXPORT_CLASS(rqt_gcs::MyPlugin, rqt_gui_cpp::Plugin)