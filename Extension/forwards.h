#ifndef __FORWARDS_H__
#define __FORWARDS_H__

#include "CDetour/detours.h"

#ifndef DETOUR_DECL_MEMBER6
#define DETOUR_DECL_MEMBER6(name, ret, p1type, p1name, p2type, p2name, p3type, p3name, p4type, p4name, p5type, p5name, p6type, p6name) \
class name##Class \
{ \
public: \
        ret name(p1type p1name, p2type p2name, p3type p3name, p4type p4name, p5type p5name, p6type p6name); \
        static ret (name##Class::* name##_Actual)(p1type, p2type, p3type, p4type, p5type, p6type); \
}; \
ret (name##Class::* name##Class::name##_Actual)(p1type, p2type, p3type, p4type, p5type, p6type) = NULL; \
ret name##Class::name(p1type p1name, p2type p2name, p3type p3name, p4type p4name, p5type p5name, p6type p6name)
#endif

#ifndef DETOUR_DECL_STATIC2
#define DETOUR_DECL_STATIC2(name, ret, p1type, p1name, p2type, p2name) \
ret (*name##_Actual)(p1type, p2type) = NULL; \
ret name(p1type p1name, p2type p2name)
#endif

bool CreateCSmokeGrenadeEmitGrenadeDetour(char *error, int maxlength);
bool CreateCFlashbangEmitGrenadeDetour(char *error, int maxlength);
bool CreateCMolotovEmitGrenadeDetour(char *error, int maxlength);
#ifndef WIN32
bool CreateCIncendiaryGrenadeEmitGrenadeDetour(char *error, int maxlength);
#endif
bool CreateCDecoyGrenadeEmitGrenadeDetour(char *error, int maxlength);
bool CreateCHEGrenadeEmitGrenadeDetour(char *error, int maxlength);
bool CreateCSmokeGrenadeProjectileSmokeDetonateDetour(char *error, int maxlength);
bool CreateCMolotovProjectileDetonateDetour(char *error, int maxlength);
bool CreateCFlashbangProjectileDetonateDetour(char *error, int maxlength);
bool CreateCCSPlayerDeafenDetour(char *error, int maxlength);
bool CreateCCSPlayerBlindDetour(char *error, int maxlength);
bool CreateCHEGrenadeProjectileDetonateDetour(char *error, int maxlength);
bool CreateCDecoyProjectileGunfireThinkDetour(char *error, int maxlength);
bool CreateCInfernoInfernoThinkDetour(char *error, int maxlength);
bool CreatePassServerEntityFilterDetour(char *error, int maxlength);

extern CDetour *g_CSmokeGrenadeEmitGrenadeDetour;
extern bool g_CSmokeGrenadeEmitGrenadeDetoured;
extern IForward *g_EmitSmokeGrenadeForward;

extern CDetour *g_CFlashbangEmitGrenadeDetour;
extern bool g_CFlashbangEmitGrenadeDetoured;
extern IForward *g_EmitFlashGrenadeForward;

extern CDetour *g_CMolotovEmitGrenadeDetour;
extern bool g_CMolotovEmitGrenadeDetoured;
#ifndef WIN32
extern CDetour *g_CIncendiaryGrenadeEmitGrenadeDetour;
extern bool g_CIncendiaryGrenadeEmitGrenadeDetoured;
#endif
extern IForward *g_EmitMolotovGrenadeForward;
extern IForward *g_EmitIncendiaryGrenadeForward;

extern CDetour *g_CDecoyGrenadeEmitGrenadeDetour;
extern bool g_CDecoyGrenadeEmitGrenadeDetoured;
extern IForward *g_EmitDecoyGrenadeForward;

extern CDetour *g_CHEGrenadeEmitGrenadeDetour;
extern bool g_CHEGrenadeEmitGrenadeDetoured;
extern IForward *g_EmitHEGrenadeForward;

extern CDetour *g_CSmokeGrenadeProjectileSmokeDetonateDetour;
extern bool g_CSmokeGrenadeProjectileSmokeDetonateDetoured;
extern IForward *g_OnSmokeGrenadeDetonateForward;

extern CDetour *g_CMolotovProjectileDetonateDetour;
extern bool g_CMolotovProjectileDetonateDetoured;
extern IForward *g_OnMolotovDetonateForward;
extern IForward *g_OnIncendiaryDetonateForward;

extern CDetour *g_CFlashbangProjectileDetonateDetour;
extern bool g_CFlashbangProjectileDetonateDetoured;
extern IForward *g_OnFlashbangDetonateForward;

extern CDetour *g_CCSPlayerDeafenDetour;
extern bool g_CCSPlayerDeafenDetoured;
extern IForward *g_OnFlashbangDeafenForward;
extern bool g_ShouldBlockDeafenHook;

extern CDetour *g_CCSPlayerBlindDetour;
extern bool g_CCSPlayerBlindDetoured;
extern IForward *g_OnFlashbangBlindForward;
extern bool g_ShouldBlockBlindHook;

extern CDetour *g_CHEGrenadeProjectileDetonateDetour;
extern bool g_CHEGrenadeProjectileDetonateDetoured;
extern IForward *g_OnHEGrenadeDetonateForward;

extern CDetour *g_CDecoyProjectileGunfireThinkDetour;
extern bool g_CDecoyProjectileGunfireThinkDetoured;

extern CDetour *g_CInfernoInfernoThinkDetour;
extern bool g_CInfernoInfernoThinkDetoured;

extern CDetour *g_PassServerEntityFilterDetour;
extern bool g_PassServerEntityFilterDetoured;

extern IForward *g_PassServerEntityFilterForward;
extern IForward *g_ShouldCollideForward;

#endif