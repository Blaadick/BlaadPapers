#include "gui/QFlowLayout.hpp"

#include <QWidget>

QFlowLayout::QFlowLayout(const int margin, const int hSpacing, const int vSpacing, QWidget *parent) : QLayout(parent) {
    m_hSpace = hSpacing;
    m_vSpace = vSpacing;
    setContentsMargins(margin, margin, margin, margin);
}

QFlowLayout::~QFlowLayout() {
    QLayoutItem *item;
    while((item = takeAt(0))) delete item;
}

void QFlowLayout::addItem(QLayoutItem *item) {
    itemList.push_back(item);
}

int QFlowLayout::horizontalSpacing() const {
    if(m_hSpace >= 0) {
        return m_hSpace;
    }

    return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
}

int QFlowLayout::verticalSpacing() const {
    if(m_vSpace >= 0) {
        return m_vSpace;
    }

    return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
}

int QFlowLayout::count() const {
    return static_cast<int>(itemList.size());
}

QLayoutItem *QFlowLayout::itemAt(const int index) const {
    return index >= 0 && index < static_cast<int>(itemList.size()) ? itemList[index] : nullptr;
}

QLayoutItem *QFlowLayout::takeAt(const int index) {
    if(index >= 0 && index < static_cast<int>(itemList.size())) {
        const auto item = itemList[index];
        itemList.erase(itemList.begin() + index);
        return item;
    }
    return nullptr;
}

Qt::Orientations QFlowLayout::expandingDirections() const {
    return {};
}

bool QFlowLayout::hasHeightForWidth() const {
    return true;
}

int QFlowLayout::heightForWidth(const int width) const {
    const int height = doLayout(QRect(0, 0, width, 0), true);
    return height;
}

void QFlowLayout::setGeometry(const QRect &rect) {
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize QFlowLayout::sizeHint() const {
    return minimumSize();
}

QSize QFlowLayout::minimumSize() const {
    QSize size;
    for(const auto item: itemList) size = size.expandedTo(item->minimumSize());

    const auto margins = contentsMargins();
    size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());
    return size;
}

int QFlowLayout::doLayout(const QRect &rect, const bool testOnly) const {
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    const QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int lineHeight = 0;

    for(const auto item: itemList) {
        const auto *wid = item->widget();
        int spaceX = horizontalSpacing();
        if(spaceX == -1)
            spaceX = wid->style()->layoutSpacing(
                QSizePolicy::PushButton,
                QSizePolicy::PushButton,
                Qt::Horizontal
            );
        int spaceY = verticalSpacing();
        if(spaceY == -1)
            spaceY = wid->style()->layoutSpacing(
                QSizePolicy::PushButton,
                QSizePolicy::PushButton,
                Qt::Vertical
            );

        int nextX = x + item->sizeHint().width() + spaceX;
        if(nextX - spaceX > effectiveRect.right() && lineHeight > 0) {
            x = effectiveRect.x();
            y = y + lineHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }

        if(!testOnly) item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

        x = nextX;
        lineHeight = qMax(lineHeight, item->sizeHint().height());
    }
    return y + lineHeight - rect.y() + bottom;
}

int QFlowLayout::smartSpacing(const QStyle::PixelMetric pm) const {
    QObject *parent = this->parent();
    if(!parent) {
        return -1;
    }

    if(parent->isWidgetType()) {
        const auto pw = dynamic_cast<QWidget *>(parent);
        return pw->style()->pixelMetric(pm, nullptr, pw);
    }

    return dynamic_cast<QLayout *>(parent)->spacing();
}
