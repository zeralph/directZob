#pragma once
// no idea where these are defined; undefine them, as reactphysics will use methods which have these macro names
#undef min
#undef max
#include <reactphysics3d/reactphysics3d.h>
#include "ZobPhysicsEngine.h"
#include "ZobPhysicComponent.h"
#include "Rendering/ZobVector3.h"
#include "DirectZob.h"
#include <vector>

using namespace reactphysics3d;

class ZobPhysicsContactsListener : public EventListener
{
public :
    ZobPhysicsContactsListener() {};
    ~ZobPhysicsContactsListener() {};
    // Override the onContact() method 
    virtual void onContact(const CollisionCallback::CallbackData& callbackData) override 
    {
        ZobPhysicsEngine* phyEng = DirectZob::GetInstance()->GetPhysicsEngine();
        // For each contact pair 
        for (uint p = 0; p < callbackData.getNbContactPairs(); p++) 
        {
            // Get the contact pair 
            CollisionCallback::ContactPair contactPair = callbackData.getContactPair(p);
            // For each contact point of the contact pair 
            ZobPhysicComponent* zobComp1 = phyEng->GetZobComponentFromRigidBody(contactPair.getBody1());
            ZobPhysicComponent* zobComp2 = phyEng->GetZobComponentFromRigidBody(contactPair.getBody2());
            assert(zobComp1 != NULL);
            assert(zobComp2 != NULL);
            if (zobComp1->GetType() != ZobPhysicComponent::ePhysicComponentType_static)
            {
                ZobPhysicComponent::collision coll;
                //only first contact for now
                CollisionCallback::ContactPoint contactPoint = contactPair.getContactPoint(0);
                Vector3 v = contactPair.getCollider1()->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider1();
                coll.worldPosition.x = v.x;
                coll.worldPosition.y = v.y;
                coll.worldPosition.z = v.z;
                v = contactPoint.getWorldNormal();
                coll.worldNormal.x = v.x;
                coll.worldNormal.y = v.y;
                coll.worldNormal.z = v.z;
                coll.penetration = contactPoint.getPenetrationDepth();
                zobComp1->OnCollide(coll);
            }
            if (zobComp2->GetType() != ZobPhysicComponent::ePhysicComponentType_static)
            {
                ZobPhysicComponent::collision coll;
                //only first contact for now
                CollisionCallback::ContactPoint contactPoint = contactPair.getContactPoint(0);
                Vector3 v = contactPair.getCollider2()->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider2();
                coll.worldPosition.x = v.x;
                coll.worldPosition.y = v.y;
                coll.worldPosition.z = v.z;
                v = contactPoint.getWorldNormal();
                coll.worldNormal.x = -v.x;
                coll.worldNormal.y = -v.y;
                coll.worldNormal.z = -v.z;
                coll.penetration = contactPoint.getPenetrationDepth();
                zobComp1->OnCollide(coll);
            }
        }
    }

private:

};