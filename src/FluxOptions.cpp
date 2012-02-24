#include "FluxOptions.h"
#include <QString>
#include <QHash>

Options::Options() {
    booleanOptions = new QHash<QString,bool>();
    doubleOptions = new QHash<QString,double>();

    registerBoolOption("showtris",false);
    registerBoolOption("showcenters",false);
}


Options::~Options() {
    delete booleanOptions;
    delete doubleOptions;
}


void Options::registerBoolOption(QString name, bool defaultValue) {
    booleanOptions->insert(name,defaultValue);
}

void Options::registerDoubleOption(QString name, double defaultValue) {
    doubleOptions->insert(name,defaultValue);
}

void Options::setBoolOption(QString name, bool value) {
    booleanOptions->insert(name, value);
}

void Options::setDoubleOption(QString name, double value) {
    doubleOptions->insert(name,value);
}

bool Options::getBoolOption(QString name, bool optionMissingValue) {
    if(booleanOptions->contains(name)) {
        return booleanOptions->value(name);
    } else {
        return optionMissingValue;
    }
}

double Options::getDoubleOption(QString name, double optionMissingValue) {
    if(doubleOptions->contains(name)) {
        return doubleOptions->value(name);
    } else {
        return optionMissingValue;
    }
}
