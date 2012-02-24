#ifndef FLUXOPTIONS_H
#define FLUXOPTIONS_H
#include <QHash>
class QString;


// im sure this class is a terrible design and i am going to be struck
// by lightening for coding it.
class Options
{
public:
    Options();
    ~Options();
    void registerBoolOption(QString name, bool defaultValue);
    void registerDoubleOption(QString name, double defaultValue);

    void setBoolOption(QString name, bool value);
    void setDoubleOption(QString name, double value);

    bool getBoolOption(QString name, bool optionMissingValue);
    double getDoubleOption(QString name, double optionMissingValue);

private:
    QHash<QString,bool> *booleanOptions;
    QHash<QString,double> *doubleOptions;
};

#endif // FLUXOPTIONS_H
