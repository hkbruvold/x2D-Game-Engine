#include "animation.h"
#include "skeleton.h"

#include <spine/spine.h>


spAnimationWrapper::spAnimationWrapper(spSkeleton *skeleton, spAnimation *anim) :
	m_self(anim),
	m_skeleton(skeleton),
	m_lastTime(0.0f),
	m_looping(false)
{
}

spAnimation *spAnimationWrapper::get()
{
	return m_self;
}

void spAnimationWrapper::setTime(const float time)
{
	m_lastTime = time;
}

float spAnimationWrapper::getTime() const
{
	return m_lastTime;
}

void spAnimationWrapper::setLooping(const bool looping)
{
	m_looping = looping;
}

bool spAnimationWrapper::getLooping() const
{
	return m_looping;
}

void spAnimationWrapper::apply(const float dt)
{
	float time = m_lastTime + dt;
	spAnimation_apply(m_self, m_skeleton, m_lastTime, time, m_looping, 0, 0);
	m_lastTime = time;
}

void spAnimationWrapper::mix(const float dt, const float alpha)
{
	float time = m_lastTime + dt;
	spAnimation_mix(m_self, m_skeleton, m_lastTime, time, m_looping, 0, 0, alpha);
	m_lastTime = time;
}

spAnimationStateDataWrapper::spAnimationStateDataWrapper(spSkeletonWrapper *skeleton) :
	m_skeleton(skeleton)
{
	m_self = spAnimationStateData_create(m_skeleton->data());
}

spAnimationStateDataWrapper::~spAnimationStateDataWrapper()
{
	m_skeleton->release();
	spAnimationStateData_dispose(m_self);
}

void spAnimationStateDataWrapper::setMix(const string &fromName, const string &toName, const float duration)
{
	spAnimationStateData_setMixByName(m_self, fromName.c_str(), toName.c_str(), duration);
}

void spAnimationStateDataWrapper::setMix(spAnimationWrapper *from, spAnimationWrapper *to, const float duration)
{
	spAnimationStateData_setMix(m_self, from->get(), to->get(), duration);
}

float spAnimationStateDataWrapper::getMix(spAnimationWrapper *from, spAnimationWrapper *to) const
{
	return spAnimationStateData_getMix(m_self, from->get(), to->get());
}

spSkeletonWrapper *spAnimationStateDataWrapper::getSkeleton() const
{
	return m_skeleton;
}

spAnimationStateData *spAnimationStateDataWrapper::get() const
{
	return m_self;
}

spAnimationStateDataWrapper *spAnimationStateDataWrapper::Factory(spSkeletonWrapper *skeleton)
{
	return skeleton != 0 ? new spAnimationStateDataWrapper(skeleton) : 0;
}

spAnimationStateWrapper::spAnimationStateWrapper(spAnimationStateDataWrapper *data) :
	m_data(data),
	m_looping(false)
{
	m_self = spAnimationState_create(data->get());
}

spAnimationStateWrapper::~spAnimationStateWrapper()
{
	m_data->release();
	spAnimationState_dispose(m_self);
}

void spAnimationStateWrapper::setLooping(const bool looping)
{
	m_looping = looping;
}

bool spAnimationStateWrapper::getLooping() const
{
	return m_looping;
}

void spAnimationStateWrapper::setAnimation(const string &name)
{
	spAnimationState_setAnimationByName(m_self, 0, name.c_str(), m_looping);
}

void spAnimationStateWrapper::setAnimation(spAnimationWrapper *anim)
{
	spAnimationState_setAnimation(m_self, 0, anim->get(), m_looping);
}

void spAnimationStateWrapper::update(const float delta)
{
	spAnimationState_update(m_self, delta);
	spAnimationState_apply(m_self, m_data->getSkeleton()->get());
}

void spAnimationStateWrapper::addAnimation(spAnimationWrapper *anim, const float delay)
{
	spAnimationState_addAnimation(m_self, 0, anim->get(), m_looping, delay);
}

void spAnimationStateWrapper::addAnimation(const string &name, const float delay)
{
	spAnimationState_addAnimationByName(m_self, 0, name.c_str(), m_looping, delay);
}

spAnimationStateWrapper *spAnimationStateWrapper::Factory(spAnimationStateDataWrapper *data)
{
	return data != 0 ? new spAnimationStateWrapper(data) : 0;
}