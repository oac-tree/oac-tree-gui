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

#include "widget_based_tests.h"

#include "test_utils.h"

#include <QApplication>

QApplication* WidgetBasedTest::m_app = nullptr;

namespace
{
// faking argc and argv
char progname[] = "testview";
char* argv[] = {&progname[0], nullptr};
int argc = 1;
}  // namespace

void WidgetBasedTest::SetUpTestSuite()
{
  m_app = new QApplication(argc, argv);
}

void WidgetBasedTest::TearDownTestSuite()
{
  delete m_app;
  m_app = nullptr;
}
