#include "model/Counter.hpp"

Counter::Counter(QObject* parent) : QObject(parent) {}

int Counter::getCount() const {
    return count;
}

void Counter::increment() {
    count += 1;
    emit countChanged(count);
}

void Counter::decrement() {
    count -= 1;
    emit countChanged(count);
}

void Counter::reset() {
    count = 0;
    emit countChanged(0);
}
