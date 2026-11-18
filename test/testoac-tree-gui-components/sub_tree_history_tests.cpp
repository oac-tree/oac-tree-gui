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

#include "oac_tree_gui/nodeeditor/sub_tree_history.h"

#include <oac_tree_gui/core/exceptions.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

//! Tests for SubTreeHistory class.
class SubTreeHistoryTest : public ::testing::Test
{
public:
  //! Returns scope identifiers of all entries.
  static std::vector<std::string> GetScopes(const SubTreeHistory& history)
  {
    std::vector<std::string> result;
    for (const auto& entry : history.GetEntries())
    {
      result.push_back(entry.scope_identifier);
    }
    return result;
  }

  //! Matches any entry.
  static bool Any(const SubTreeHistoryEntry&) { return true; }
};

TEST_F(SubTreeHistoryTest, InitialState)
{
  const SubTreeHistory history;
  EXPECT_TRUE(history.GetEntries().empty());
  EXPECT_EQ(history.GetCurrentIndex(), 0);
  EXPECT_EQ(history.GetTrailStart(), 0);
  EXPECT_FALSE(history.FindPrevious(Any).has_value());
  EXPECT_FALSE(history.FindNext(Any).has_value());
}

TEST_F(SubTreeHistoryTest, PushAndReset)
{
  SubTreeHistory history;
  history.Push({"a", ""});
  EXPECT_EQ(GetScopes(history), std::vector<std::string>({"a"}));
  EXPECT_EQ(history.GetCurrentIndex(), 0);

  history.Push({"b", ""});
  EXPECT_EQ(GetScopes(history), std::vector<std::string>({"a", "b"}));
  EXPECT_EQ(history.GetCurrentIndex(), 1);

  history.Reset({"c", ""});
  EXPECT_EQ(GetScopes(history), std::vector<std::string>({"c"}));
  EXPECT_EQ(history.GetCurrentIndex(), 0);

  history.Clear();
  EXPECT_TRUE(history.GetEntries().empty());
  EXPECT_EQ(history.GetCurrentIndex(), 0);
}

//! A push after going back drops forward entries.
TEST_F(SubTreeHistoryTest, PushDropsForwardEntries)
{
  SubTreeHistory history;
  history.Reset({"a", ""});
  history.Push({"b", ""});
  history.Push({"c", ""});

  history.SetCurrentIndex(0);
  history.Push({"d", ""});
  EXPECT_EQ(GetScopes(history), std::vector<std::string>({"a", "d"}));
  EXPECT_EQ(history.GetCurrentIndex(), 1);

  EXPECT_THROW(history.SetCurrentIndex(2), RuntimeException);
}

//! FindPrevious and FindNext skip entries not matching.
TEST_F(SubTreeHistoryTest, FindPreviousAndNext)
{
  SubTreeHistory history;
  history.Reset({"a", ""});
  history.Push({"deleted", ""});
  history.Push({"b", ""});
  history.Push({"deleted", ""});
  history.Push({"c", ""});

  auto is_valid = [](const SubTreeHistoryEntry& entry)
  { return entry.scope_identifier != "deleted"; };

  EXPECT_EQ(history.FindPrevious(is_valid), 2);
  EXPECT_FALSE(history.FindNext(is_valid).has_value());

  history.SetCurrentIndex(2);
  EXPECT_EQ(history.FindPrevious(is_valid), 0);
  EXPECT_EQ(history.FindNext(is_valid), 4);

  history.SetCurrentIndex(0);
  EXPECT_FALSE(history.FindPrevious(Any).has_value());
  EXPECT_EQ(history.FindNext(Any), 1);
}

//! A trail starts at the last entry up to the current one without an Include.
TEST_F(SubTreeHistoryTest, TrailStart)
{
  SubTreeHistory history;
  history.Reset({"a", ""});
  history.Push({"b", "include_b"});
  EXPECT_EQ(history.GetTrailStart(), 0);

  history.Push({"c", ""});
  history.Push({"d", "include_d"});
  history.Push({"e", "include_e"});
  EXPECT_EQ(history.GetTrailStart(), 2);

  history.SetCurrentIndex(2);
  EXPECT_EQ(history.GetTrailStart(), 2);

  history.SetCurrentIndex(1);
  EXPECT_EQ(history.GetTrailStart(), 0);
}

}  // namespace oac_tree_gui::test
