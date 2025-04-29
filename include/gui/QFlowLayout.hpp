#pragma once

#include <QLayout>
#include <QStyle>
#include <QWidgetItem>
#include <vector>

class QFlowLayout final : public QLayout {
    Q_OBJECT

public:
    explicit QFlowLayout(int margin = 0, int hSpacing = -1, int vSpacing = -1, QWidget* parent = nullptr);

    ~QFlowLayout() override;

    void addItem(QLayoutItem* item) override;

    int horizontalSpacing() const;

    int verticalSpacing() const;

    Qt::Orientations expandingDirections() const override;

    bool hasHeightForWidth() const override;

    int heightForWidth(int) const override;

    int count() const override;

    QLayoutItem* itemAt(int index) const override;

    QSize minimumSize() const override;

    void setGeometry(const QRect& rect) override;

    QSize sizeHint() const override;

    QLayoutItem* takeAt(int index) override;

private:
    std::vector<QLayoutItem*> itemList;
    int m_hSpace;
    int m_vSpace;

    int doLayout(const QRect& rect, bool testOnly) const;

    int smartSpacing(QStyle::PixelMetric pm) const;
};
