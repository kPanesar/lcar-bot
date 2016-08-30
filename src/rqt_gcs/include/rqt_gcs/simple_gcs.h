#ifndef rqt_gcs__SimpleGCS_H
#define rqt_gcs__SimpleGCS_H

#include <pluginlib/class_list_macros.h>
#include <rqt_gui_cpp/plugin.h>
#include <rqt_gcs/simple_control.h>
#include <rqt_gcs/unanswered_queries.h>
#include <rqt_gcs/settings_widget.h>
#include <rqt_gcs/access_points.h>

#include <ros/ros.h>
#include <ros/common.h>
#include <std_srvs/Empty.h>
#include <image_transport/image_transport.h>
#include <lcar_msgs/Door.h>
#include <lcar_msgs/TargetLocal.h>
#include <lcar_msgs/TargetGlobal.h>

#include <ui_WidgetMainMap.h>
#include <ui_PictureMsg.h>
#include <ui_UAVCondition.h>

#include <QTimer>
#include <QThread>
#include <QMenuBar>
#include <QSettings>
#include <QSignalMapper>
#include <QWaitCondition>
#include <QMutex>

namespace rqt_gcs{
    
  #define MAX_UAV 100 // the total number of UAV's manageable by our system

  class SimpleGCSHelper;
  class UnansweredQueries;
  class SettingsWidget;
  class AccessPoints;
  
  class SimpleGCS : public rqt_gui_cpp::Plugin
  {
  Q_OBJECT

  friend class SimpleGCSHelper;
  friend class UnansweredQueries;
  friend class SettingsWidget;
  
  public:
      
//      struct UavWidget{
//          
//          UavWidget(Ui::UAVConditionWidget* ui, QWidget* widget, SimpleControl* sc)
//          {
//              this->ui = ui;
//              this->widget = widget;
//              button = ui->VehicleSelectButton;
//              this->sc = sc;
//          }
//          
//          Ui::UAVConditionWidget* ui;
//          QWidget * widget;
//          QPushButton * button;
//          SimpleControl* sc;
//      };
      
    SimpleGCS();
    ~SimpleGCS();
    ros::NodeHandle nh;
    ros::ServiceServer server;
    lcar_msgs::Door msg;
   
    image_transport::ImageTransport it_stereo{nh};
    void GetMessage(const geometry_msgs::PoseWithCovarianceStamped& msg);
    void ImageCallback(const sensor_msgs::ImageConstPtr& msg);
    void ReceivedObjectDetectionRequest(const std_msgs::Int32ConstPtr& msg);

    virtual void initPlugin(qt_gui_cpp::PluginContext& context);
    virtual void shutdownPlugin();
    virtual void saveSettings(qt_gui_cpp::Settings& plugin_settings, qt_gui_cpp::Settings& instance_settings) const;
    virtual void restoreSettings(const qt_gui_cpp::Settings& plugin_settings, const qt_gui_cpp::Settings& instance_settings);

    //methods for publishing object detection paramerter updates
    void publishHitThreshold(double thresh);
    void publishStepSize(int step);
    void publishPadding(int padding);
    void publishScaleFactor(double scale);
    void publishMeanShift(bool on);
    
    std::string GetMissionType(std::string file_name);		 
    lcar_msgs::TargetLocal GetMissionLocal(std::string file_name);
    lcar_msgs::TargetGlobal GetMissionGlobal(std::string file_name);

    static void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    
  protected slots:
    virtual void timedUpdate();
    
    //////////// Buttons
    virtual void executePlay();
    virtual void cancelPlay();
    virtual void scoutBuilding();
    virtual void stopScout();
    virtual void changeFlightMode(int);
    virtual void uavSelected(QWidget*);
    virtual void armOrDisarmSelectedUav();
    virtual void pauseOrResumeScout();
    virtual void acceptDoorQuery(QWidget *);
    virtual void rejectDoorQuery(QWidget *);
    virtual void acessPointsTriggered();
    virtual void settingsTriggered();
    virtual void unansweredQueriesTriggered();
    
    virtual void addUav(int);
    virtual void deleteUav(int);
    virtual void uavConnectionToggled(int, int, bool);

    //SETTINGS RELATED
    virtual void toggleMachineLearningMode(bool);

  private:
    
    void initMap();
    void initMenuBar();
    void initSettings();
    void initHelperThread();
      
    void selectUav(int);  
    void updatePFD();
    
    void updateQueries();
    void clearQueries();
    void saveUavQueries(SimpleControl *, QString ap_type);
    void answerQuery(QWidget *, QString ap_type, bool);
    
    void toggleScoutButtons(bool visible, QString icon_type = "pause");
    void toggleArmDisarmButton(bool arm);
    
    int cur_uav;
    int timeCounter;
    int NUM_UAV; //Total number of UAV's in the system
    int num_queries_last;
    
    struct ObjectDetectionMessageHandlers // publishers and subscribers
    {
        ros::Publisher pub_hit_thresh;
        ros::Publisher pub_step_size;
        ros::Publisher pub_padding;
        ros::Publisher pub_scale_factor;
        ros::Publisher pub_mean_shift;
        ros::Subscriber sub_od_request;
    } od_handlers;
    
    struct ObjectDetectionParamaters // object detection parameters
    {
        //defaults
        double hit_thresh = 0; // displayed as a decimal
        int step_size = 16;
        int padding = 8;
        double scale_factor = 1.15; // displayed as a decimal
        bool mean_shift = false;
    } od_params;
    
    struct FloatingWidgets 
    {
        SettingsWidget * settings = nullptr;
        UnansweredQueries * unanswered_queries = nullptr;
        AccessPoints* ap_menu = nullptr;
    } fl_widgets_;
    
    QVector<SimpleControl*> active_uavs;
    QMap<int, SimpleControl*> uav_db;

    std::vector<lcar_msgs::DoorPtr> * vec_uav_queries_;

    image_transport::Subscriber sub_stereo;
    Ui::centralWidget central_ui_;
    QWidget* central_widget_;
    QVector<QWidget*> vec_uav_list_widget_;
    QVector<Ui::UAVConditionWidget*> vec_uav_list_ui_;
    std::vector<QWidget*> query_widgets_; // recieves SimpleControl::getRefQuerires
     
    QMenuBar * menu_bar_;
    
    QMenu * file_menu;
    QAction * start_uav_act;
    QAction * start_uav_group_act;
    QAction * shutdown_uav_act;
    QAction * shutdown_uav_group_act;
    
    QMenu * view_menu;
    QAction * unanswered_queries_act;
    
    QMenu * tools_menu;
    QAction* settings_act;
    
    QMenu * help_menu;
    QAction * about_act;
    QAction * lcar_bot_act;
    QAction * ros_act;
    QAction * qt_act;
    QAction * opencv_act;
    QTimer* update_timer;

    QString temp_data;
    QSignalMapper* uav_select_mapper;
    QSignalMapper* accept_door_mapper;
    QSignalMapper* deny_door_mapper;

    QSettings* settings_;

    QThread t_uav_monitor;
    SimpleGCSHelper * uav_monitor;
    QMutex uav_mutex;
    QWaitCondition num_uav_changed;
  };

  class SimpleGCSHelper : public QObject
  {
  Q_OBJECT

  public:
      SimpleGCSHelper(SimpleGCS *);
      ~SimpleGCSHelper();

  public slots:
      void help();
      
  signals:
      void addUav(int); // uav_id
      void deleteUav(int);
      void toggleUavConnection(int, int, bool);
      
  private:
      SimpleGCS * gcs;

      void parseUavNamespace(std::map<int, int>&);

      void monitorUavNamespace();
      void monitorUavConnections();
      void runUavs();
  };

} // rqt_gcs name space
#endif //rqt_gcs__SimpleGCS_H
