#ifndef SEQUENCEITEM_H
#define SEQUENCEITEM_H

#include "CompoundItem.h"

namespace Sequencer {

namespace Model {

/**
 * @brief The SequenceItem class
 *
 * @details This Item contains a list of subitems that
 * represent instructions to execute, variable definitions, etc.
 */
class SequenceItem : public CompoundItem
{
public:
    /**
     * @brief Constructor
     */
    SequenceItem();

    /**
     * @brief Destructor
     */
    ~SequenceItem() override;
};

}  // namespace Model

}  // namespace Sequencer

#endif // SEQUENCEITEM_H
