/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "filtered_proxy_viewmodel.h"

namespace sequencergui
{

FilteredProxyViewModel::FilteredProxyViewModel(QObject *parent) : QSortFilterProxyModel(parent) {}

void FilteredProxyViewModel::SetPattern(const QString &pattern)
{
  m_pattern = pattern.toLower();
  invalidateFilter();
}

bool FilteredProxyViewModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
  const QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);

  return sourceModel()->data(index0).toString().toLower().contains(m_pattern);
}

}  // namespace sequencergui
