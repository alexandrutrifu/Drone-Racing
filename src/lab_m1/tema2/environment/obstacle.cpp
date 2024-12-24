#include "obstacle.h"

bool obstacles::Obstacle::overlapsWith(BoundingBox *boundingBox)
{
    return this->boundingBox->xLimits.x <= boundingBox->xLimits.y && this->boundingBox->xLimits.y >= boundingBox->xLimits.x &&
        this->boundingBox->yLimits.x <= boundingBox->yLimits.y && this->boundingBox->yLimits.y >= boundingBox->yLimits.x &&
        this->boundingBox->zLimits.x <= boundingBox->zLimits.y && this->boundingBox->zLimits.y >= boundingBox->zLimits.x;
}