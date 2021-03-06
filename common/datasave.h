#ifndef DATASAVE_H
#define DATASAVE_H
#include <QApplication>

//标定相关：
//记录标定了几组数据。0-12 //TODO 标定样计数值需要加一
#define MYSETTINGS_CALIBRATE_COUNT "calibratemeasurement_count"
#define MYSETTINGS_CALIBRATE_S_DATA(x) QString("s_count_data_").append(QString::number((x)))
#define MYSETTINGS_CALIBRATE_S_INPUT(x) QString("calibrate_input_s_").append(QString::number((x)))
//relavnt calibrate results
//标定数据下次保存的序号.1-20
#define MYSETTINGS_CALIBRATE_WORK_CURVE(x) QString("work_curve_%1").arg(x)
#define MYSETTINGS_CALIBRATE_RESULT_COUNT "calibration_results_count"
#define MYSETTINGS_CALIBRATE_RESULT_RESULT(x) QString("calibration_results_in_result_").append(QString::number((x)))
#define MYSETTINGS_CALIBRATE_RESULT_DATA(x) QString("calibration_results_in_data_").append(QString::number((x)))
#define MYSETTINGS_CALIBRATE_RESULT_REAL_KBR(x) QString("real_compute_kbr_1").append(QString::number((x)))

//计数相关
#define MYSETTINGS_COUNT_MAX_COUNT 1000
#define MYSETTINGS_COUNT_COUNT QString("count_count")
#define MYSETTINGS_COUNT_DATA(x) QString("count_data").append(QString::number((x)))
#define MYSETTINGS_COUNT_DATA_LAMBDA(x) QString("count_data_lambda_").append(QString::number((x)))
#define MYSETTINGS_COUNT_DATA_AVERAGE(x) QString("count_data_average_").append(QString::number((x)))
#define MYSETTINGS_COUNT_DATA_DATETIME(x) QString("count_data_datetime_").append(QString::number((x)))

//含量测量相关：
#define MYSETTINGS_SAMPLE_COUNT QString("sample_count")
#define MYSETTINGS_SAMPLE_DATA(x) QString("sample_data_").append(QString::number((x)))

//其他相关：
#define MYSETTINGS_SERIAL "product_serial"

#define MYSETTINGS_USED_TIME "machine_used_time"    //机器使用时间
#define MYSETTINGS_MAXIMAL_USE_TIME "most_use_time"

//计数管电压
#define MYSETTINGS_COUNT_VOLTAGE "count_voltage"
#define MYSETTINGS_CHANGE_COUNT_VOLTAGE_COUNT "change_count_voltage_count"
#define MYSETTINGS_CHANGE_COUNT_VOLTAGE(x) QString("change_count_voltage_").append(QString::number((x)))

//光管电压和光管电流
#define MYSETTINGS_LIGHT_VOLTAGE "light_voltage"
#define MYSETTINGS_LIGHT_CURRENT "light_current"

#define MYSETTINGS_PROPORTION(x) QString("proportion_").append(QString::number((x)))

#define MYSETTINGS_COM_ERR(x) QString("com_err").append(QString::number((x)))

class QSettings;
class AbstractDataSave
{
public:
    virtual QVariant value(QString key);
    virtual void setValue(QString key, qint64 value);
    virtual void setValue(QString key, QString value);
    virtual void addCount(uint /*which*/);
    virtual void remove(QString key);
    virtual bool contains(QString key);

protected:
    QSettings *settings;

    bool test();
};

class MeasurementDataSave : public AbstractDataSave
{

public:
    static MeasurementDataSave *instance();
private:
    MeasurementDataSave();
    void init();
};

class ErrorCountSave : public AbstractDataSave
{
public:
    static ErrorCountSave *instance();
    void addCount(uint which) Q_DECL_OVERRIDE;

private:
    ErrorCountSave();
    void init();
};

class CountDataSave : public AbstractDataSave
{
public:
    static CountDataSave *instance();
private:
    CountDataSave();
    void init();
};

class CalibrateDataSave : public AbstractDataSave
{
public:
    static CalibrateDataSave *instance();
private:
    CalibrateDataSave();
    void init();
};
#endif // DATASAVE_H
