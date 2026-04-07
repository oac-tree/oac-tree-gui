/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

// Copyright (C) 2025 Jarek Kobus
// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
//
// Copy of progress indicator from Qt's TaskTree demo in examples/widgets/demo/progressindicator.

#include "busy_sign.h"

#include <QEvent>
#include <QPainter>
#include <QTimer>
#include <QWidget>

namespace oac_tree_gui
{

class OverlayWidget : public QWidget
{
public:
  using PaintFunction = std::function<void(QWidget*, QPainter&, QPaintEvent*)>;

  explicit OverlayWidget(QWidget* parent = nullptr)
  {
    setAttribute(Qt::WA_TransparentForMouseEvents);
    if (parent != nullptr)
    {
      attachToWidget(parent);
    }
  }

  void attachToWidget(QWidget* parent)
  {
    if (parentWidget() != nullptr)
    {
      parentWidget()->removeEventFilter(this);
    }
    setParent(parent);
    if (parent != nullptr)
    {
      parent->installEventFilter(this);
      resizeToParent();
      raise();
    }
  }
  void setPaintFunction(PaintFunction&& paint) { m_paint_func = std::move(paint); }

protected:
  bool eventFilter(QObject* obj, QEvent* event) override
  {
    if ((obj == parent()) && (event->type() == QEvent::Resize))
    {
      resizeToParent();
    }
    return QWidget::eventFilter(obj, event);
  }

  void paintEvent(QPaintEvent* event) override
  {
    if (m_paint_func)
    {
      QPainter p(this);
      m_paint_func(this, p, event);
    }
  }

private:
  void resizeToParent() { setGeometry(QRect(QPoint(0, 0), parentWidget()->size())); }

  PaintFunction m_paint_func;
};

class BusySignPainter
{
public:
  using UpdateCallback = std::function<void()>;

  BusySignPainter();

  void setUpdateCallback(UpdateCallback&& cb) { m_callback = std::move(cb); }

  QSize size() const { return m_pixmap.size() / m_pixmap.devicePixelRatio(); }

  void paint(QPainter& painter, const QRect& rect) const;
  void startAnimation() { m_timer.start(); }
  void stopAnimation() { m_timer.stop(); }

protected:
  void nextAnimationStep()
  {
    const int degrees_in_circle = 360;
    m_rotation = (m_rotation + m_rotation_step + degrees_in_circle) % degrees_in_circle;
  }

private:
  const int m_rotation_step{45};
  int m_rotation{0};
  QTimer m_timer;
  QPixmap m_pixmap;
  UpdateCallback m_callback;
};

BusySignPainter::BusySignPainter()
{
  m_timer.setSingleShot(false);

  auto on_timeout = [this]
  {
    nextAnimationStep();
    if (m_callback)
    {
      m_callback();
    }
  };
  QObject::connect(&m_timer, &QTimer::timeout, &m_timer, on_timeout);

  m_timer.setInterval(100);
  m_pixmap = QPixmap(QString(":/oac-tree/icons/progressindicator.png"));
}

void BusySignPainter::paint(QPainter& painter, const QRect& rect) const
{
  painter.save();
  painter.setRenderHint(QPainter::SmoothPixmapTransform);
  const QPoint translate(rect.x() + (rect.width() / 2), rect.y() + (rect.height() / 2));
  QTransform t;
  t.translate(translate.x(), translate.y());
  t.rotate(m_rotation);
  t.translate(-translate.x(), -translate.y());
  painter.setTransform(t);
  const QSize pixmapUserSize(m_pixmap.size() / m_pixmap.devicePixelRatio());
  painter.drawPixmap(QPoint(rect.x() + ((rect.width() - pixmapUserSize.width()) / 2),
                            rect.y() + ((rect.height() - pixmapUserSize.height()) / 2)),
                     m_pixmap);
  painter.restore();
}

class BusySignWidget : public OverlayWidget
{
public:
  explicit BusySignWidget(QWidget* parent = nullptr) : OverlayWidget(parent)
  {
    auto paint_func = [this](QWidget* widget, QPainter& painter, QPaintEvent* event)
    {
      (void)event;
      m_paint.paint(painter, widget->rect());
    };
    setPaintFunction(paint_func);
    m_paint.setUpdateCallback([this] { update(); });
    updateGeometry();
  }

  QSize sizeHint() const final { return m_paint.size(); }

  void SetAnimationEnabled(bool value)
  {
    m_animation_enabled = value;
    UpdateAnimation();
  }

protected:
  void showEvent(QShowEvent* event) final
  {
    (void)event;
    UpdateAnimation();
  }

  void hideEvent(QHideEvent* event) final
  {
    (void)event;
    UpdateAnimation();
  }

private:
  /**
   * @brief Starts/stop animation depending on m_animation_enabled and widget visibility.
   */
  void UpdateAnimation()
  {
    if (m_animation_enabled && isVisible())
    {
      m_paint.startAnimation();
    }
    else
    {
      m_paint.stopAnimation();
    }
  }
  BusySignPainter m_paint;
  bool m_animation_enabled{false};
};

BusySign::BusySign(QWidget* parent_widget)
    : QObject(parent_widget), m_widget(new BusySignWidget(parent_widget))
{
}

void BusySign::SetIndicatorType(BusySignType indicator_type)
{
  m_widget->setVisible(indicator_type != BusySignType::kHidden);
  m_widget->SetAnimationEnabled(indicator_type == BusySignType::kAnimated);
}

}  // namespace oac_tree_gui
