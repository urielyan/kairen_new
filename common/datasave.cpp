#include "datasave.h"
#include "global.h"

bool MeasurementDataSave::test()
{
  QString testKey = "testThisClass";
//  setValue(testKey, 1);
  settings->setValue(testKey, 1);
  qDebug() <<  settings->value(testKey) << settings->fileName();
  if(settings->value(testKey).toInt() != 1)
    {
      PRINT_DEBUG_INFOR;
      qDebug() << "QSettings err!";

      return false;
  }
  settings->remove(testKey);

  return true;
}

void MeasurementDataSave::setValue(QString key, qint64 value)
{
  if(settings == NULL)
    return;
  settings->setValue(key, value);
}

void MeasurementDataSave::setValue(QString key, QString value)
{
  if(settings == NULL)
    return;
  settings->setValue(key, value);
}

void MeasurementDataSave::addCount(uint)
{
  qDebug() << "Not support";
}

QVariant MeasurementDataSave::value(QString key)
{
  if(settings == NULL)
    return 0;
  return settings->value(key);
}


MeasurementDataSave *MeasurementDataSave::instance()
{
  static MeasurementDataSave _instance;
  return &_instance;
}

MeasurementDataSave::MeasurementDataSave()
{
  settings = new QSettings("./QSettings/measurementData.settings", QSettings::NativeFormat);
  if(!test())
    {
      settings = new QSettings("./QSettings/measurementData.settings", QSettings::NativeFormat);
    }
  init();

  QString workCurve = settings->value(MYSETTINGS_CALIBRATE_RESULT_REAL_KBR(2), ";;").toString();

  if((workCurve.split(";")[0] == NULL) || (workCurve.split(";")[1] == NULL)){
     qDebug() << "err";
      return;
  }

  Q_ASSERT(test() == true);
}

void MeasurementDataSave::init()
{
  if(!settings->contains("calibration_results_count")){
      settings->setValue("calibration_results_count",1);
    }
  if((!settings->contains("calibration_results_in_result_1"))
     && (!settings->contains("calibration_results_in_data_10"))){
      for(int tmp = 1;tmp <= 10;tmp++){
          //qDebug() << "calibrate_results";
          settings->setValue(QString("calibration_results_in_result_%1").arg(tmp)," ; ; ; ");
          settings->setValue(QString("calibration_results_in_data_%1").arg(tmp),"");
        }
    }

  QString tmpstr = "calibrate_input_s_";
  if(!settings->contains("calibrate_input_s_1")){
      for(int tmpnumber = 0; tmpnumber < 12 ;tmpnumber++){
          tmpstr.append(QString("%1").arg(tmpnumber));
          settings->setValue(tmpstr,"0.0000");
          tmpstr = "calibrate_input_s_";
        }
    }


  tmpstr = "s_count_data_";
  if(!settings->contains("s_count_data_1")){
      for(int tmpnumber = 0; tmpnumber < 12 ;tmpnumber++){
          tmpstr.append(QString("%1").arg(tmpnumber));
          qDebug() << tmpstr;
          settings->setValue(tmpstr,"");
          tmpstr = "s_count_data_";
      }
  }

  if(!settings->contains("work_curve_1") || !settings->contains("work_curve_9")){
      for(int i = 1;i <= 5 ; i++){
          settings->setValue(QString("work_curve_%1").arg(i),";;");
        }
      for(int i = 6 ; i <= 9 ; i++){
          settings->setValue(QString("work_curve_%1").arg(i),";;");
        }
    }


  //judge calibratemeasurement_count  whether is exit;if not exit create it.finally assign count variable.
  if(!settings->contains("calibratemeasurement_count")){
      settings->setValue("calibratemeasurement_count",0);
    }

  if(!settings->contains("count_voltage")){
      settings->setValue("count_voltage",578);
    }

  if(!settings->contains("light_voltage")){
      settings->setValue("light_voltage"," ");
    }

  if(!settings->contains("light_current")){
      settings->setValue("light_current"," ");
    }


  if(!settings->contains("machine_used_time")){
      settings->setValue("machine_used_time",0);
    }
  if(!settings->contains("most_use_time")){
      settings->setValue("most_use_time",0);
    }
  if (!settings->contains("passwd")){
      settings->setValue("passwd",111111);
  }

  // not set default value
//    if(!settings->contains("proportion_1"))
//      {
//        settings->setValue("proportion_1", 0.1);
//        settings->setValue("proportion_2", 1);
//      }


  if(!settings->contains("sample_count")){
      settings->setValue("sample_count",0);
  }

  if(!settings->contains("count_count")){
      settings->setValue("count_count",0);
  }
}


ErrorCountSave *ErrorCountSave::instance()
{
  static ErrorCountSave _instance;
  return &_instance;
}

void ErrorCountSave::init()
{
  settings = new QSettings("./QSettings/errorCount.settings", QSettings::NativeFormat);

}


void ErrorCountSave::addCount(uint which)
{
  QString key = QString("com_err_") + QString::number(which);
  settings->setValue(key, settings->value(key).toUInt() + 1);
}

ErrorCountSave::ErrorCountSave()
{
  init();
  if(!settings->contains("com_err_1")){
      for(int i = 1;i <= 11;i++){
          settings->setValue(QString("com_err_%1").arg(i),0);
        }
    }

  if(!settings->contains("change_count_voltage_count")){
      settings->setValue("change_count_voltage_count",1);
    }
}

bool ErrorCountSave::test()
{
  QString testKey = "test";
  setValue(testKey, 1);
  qDebug() <<  settings->value(testKey) << settings->fileName();
  if(settings->value(testKey).toInt() != 1)
    {
      PRINT_DEBUG_INFOR;
      qDebug() << "QSettings err!";

      return false;
  }
  settings->remove(testKey);

  return true;
}

void ErrorCountSave::setValue(QString key, qint64 value)
{
  if(settings == NULL)
    return;
  settings->setValue(key, value);
}

void ErrorCountSave::setValue(QString key, QString value)
{
  if(settings == NULL)
    return;
  settings->setValue(key, value);
}

QVariant ErrorCountSave::value(QString key)
{
  if(settings == NULL)
    return 0;
  return settings->value(key);
}

bool AbstractDataSave::contains(QString key)
{
    if(settings == NULL)
    {
        return false;
    }

    return settings->contains(key);
}

void AbstractDataSave::remove(QString key)
{
    if(settings == NULL)
    {
        return;
    }

    return settings->remove(key);
}
