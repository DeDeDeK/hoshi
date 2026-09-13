#ifndef KAR_H_DEBUG
#define KAR_H_DEBUG

// Two hemisphere caps and a cylinder spanning pos_prev..pos_cur; a bare sphere if they are equal.
void DebugGX_DrawSphere(float radius, Vec3 *pos_prev, Vec3 *pos_cur, GXColor *color_prev, GXColor *color_cur); // 0x8007d988

#endif