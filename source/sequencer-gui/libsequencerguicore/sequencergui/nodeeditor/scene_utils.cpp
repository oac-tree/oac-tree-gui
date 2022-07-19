/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/nodeeditor/scene_utils.h"

#include <mvvm/core/exceptions.h>
#include <sequencergui/model/aggregate_factory.h>
#include <sequencergui/model/domain_utils.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/sequencer_utils.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/transform_from_domain.h>
#include <sequencergui/utils/style_utils.h>

#include <QDebug>
#include <QLinearGradient>
#include <QRectF>
#include <numeric>

namespace sequencergui
{

QSizeF GetGraphicsViewportSize()
{
  return {6000.0, 6000.0};
}

QPointF GetGraphicsViewportOrigin()
{
  return {-3000.0, 0.0};
}

QPointF GetGraphicsViewportCenter()
{
  // y-axis on graphics scene is pointing downwards. Our scene's corners looks like this:
  // (-3000, 0)     (3000, 0)
  // (-3000, 6000)  (3000, 6000)
  const auto origin = GetGraphicsViewportOrigin();
  const auto size = GetGraphicsViewportSize();
  return {origin.x() + size.width() / 2, origin.y() + size.height() / 2};
}

double GetAlignmentGridWidth()
{
  return sequencergui::ConnectableViewRectangle().width() * 1.4;
}

double GetAlignmentGridHeight()
{
  return sequencergui::ConnectableViewRectangle().height() * 1.4;
}

QGradient ConnectableViewGradient(const QColor& color, const QRectF& rect)
{
  QLinearGradient result(rect.x() + rect.width() / 2, rect.y(), rect.x() + rect.width() / 2,
                         rect.y() + rect.height());
  result.setColorAt(0, color);
  result.setColorAt(0.5, color.lighter(150));
  result.setColorAt(1, color);
  return result;
}

QRectF ConnectableViewRectangle()
{
  // make size of rectangle depending on 'M'-letter size to address scaling issues
  static QRectF result = QRectF(0, 0, styleutils::UnitSize(8.2), styleutils::UnitSize(10.0));
  return result;
}

std::vector<QPointF> GetPositions(const QPointF& reference, int n_points, double width)
{
  std::vector<QPointF> result;
  if (n_points < 0)
  {
    throw std::runtime_error("Error in GetPositions: invalid number of points");
  }

  double xpos = reference.x() - (n_points - 1) * width / 2;
  for (int i = 0; i < n_points; ++i)
  {
    result.emplace_back(QPointF(xpos, reference.y()));
    xpos += width;
  }
  return result;
}

QColor GetBaseColor(const InstructionItem* instruction)
{
  if (instruction->GetType() == UnknownInstructionItem::Type)
  {
    return {"chartreuse"};
  }

  if (IsDecoratorInstruction(instruction))
  {
    return {"lightseagreen"};
  }

  if (IsCompoundInstruction(instruction))
  {
    return {"royalblue"};
  }

  return {Qt::lightGray};
}

InstructionItem* AddSingleInstruction(SequencerModel* model, InstructionContainerItem* container,
                                      const std::string& domain_type)
{
  try
  {
    auto item_type = GetItemType(domain_type);
    return dynamic_cast<InstructionItem*>(
          model->InsertItem(model->GetFactory()->CreateItem(item_type), container, {}));
  }
  catch (const mvvm::NotFoundKeyException& ex)
  {
    // The UI knows nothing about instruction of given type.
    return dynamic_cast<InstructionItem*>(
        model->InsertItem(CreateUnknownInstructionItem(domain_type), container, {}));
  }
}

InstructionItem* AddAggregate(SequencerModel* model, InstructionContainerItem* container,
                              const std::string& aggregate_name)
{
  AggregateFactory factory;
  auto factory_func = factory.GetValue(aggregate_name);
  return dynamic_cast<InstructionItem*>(model->InsertItem(factory_func(), container, {}));
}

}  // namespace sequencergui
