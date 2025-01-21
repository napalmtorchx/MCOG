xvs.1.0
#pragma screenspace

; shader declaration
;
; - STREAM0:
; - v0      : Position(Vector3f)
; - v1      : Color(DWORD)
; - v2	    : Normal(Vector3f)
; - v6      : TexCoord(Vector2f)
; - c0-c3   : Vector3f(0, 0, 0, 0)
; - c20-c23 : World * View(Matrix)
; - c30-c33 : World * View * Projection(Matrix)
; - c40	    : Light
; - c44	    : Fog
; - c50-c53 : FaceNormal_Front(Vector3f) 
; - c54-c57 : FaceNormal_Right(Vector3f) 
; - c58-c61 : FaceNormal_Back(Vector3f) 
; - c62-c65 : FaceNormal_Left(Vector3f) 
; - c66-c69 : FaceNormal_Top(Vector3f) 
; - c70-c73 : FaceNormal_Bottom(Vector3f) 

; ---- Transform matrix ---------------------------------------------
m4x4	oPos, v0, c30   ; oPos = Position * WorldViewProjection

; ---- Copy color ---------------------------------------------------
mov oD0, v1                     ; oD0    = Color
mov oT0.xy, v6		        ; oT0.xy = TexCoord

; Transform vertex to view space
dp4 r0.z, v0, c22

; Perform linear fog equation
mad oFog.x, r0.z, c44.x, c44.y


;------------------------------------------------------------------------------
; Final stuff
;------------------------------------------------------------------------------

; Screenspace instructions are interwoven to prevent a stall
mul oPos.xyz, r12, c-38
+rcc r1.x, r12.w

mad oPos.xyz, r12, r1.x, c-37
