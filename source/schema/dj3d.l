/* dj3d.l */
%{
    #include "dj3d_parser.h"
    #include <stdlib.h>
%}


%%
\".*\"                                      { yylval.str = yytext; return STRING; }
[-+]?[0-9]+\.[0-9]+                         { yylval.f32 = (float)atof(yytext); printf("FLOAT "); return FLOAT; }
[0-9]+                                      { yylval.u32 = (unsigned int)atoi(yytext); printf("UINT "); return UINT; }
("<")/[A-z]                                 { printf("LT "); return LT; }
("</")/[A-z]                                { printf("LTS "); return LTS; }
">"                                         { printf("GT "); return GT; }
"="                                         { printf("EQUAL "); return EQUAL; }
","                                         { printf("COMMA "); return COMMA; }
"id"|"ID"|"Id"                              { printf("ID "); return ID; }
"attr"|"ATTR"|"Attr"                        { printf("ATTR "); return ATTR; }
"layer"|"LAYER"|"Layer"                     { printf("LAYER "); return LAYER; }
"textid"|"TEXTID"|"TextId"                  { printf("TEXTID "); return TEXTID; }
"op"|"OP"|"Op"                              { printf("OP "); return OP; }
"count"|"COUNT"|"Count"                     { printf("COUNT "); return COUNT; }
"fovy"|"FOVY"|"Fovy"                        { printf("FOVY "); return FOVY; }
"zn"|"ZN"|"Zn"                              { printf("ZN "); return ZN; }
"zf"|"ZF"|"Zf"                              { printf("ZF "); return ZF; }
"status"|"STATUS"|"Status"                  { printf("STATUS "); return STATUS; }
"on"|"ON"|"On"                              { printf("ON "); return ON; }
"off"|"OFF"|"Off"                           { printf("OFF "); return OFF; }
"vertex"|"VERTEX"|"Vertex"                  { printf("VERTEX "); return VERTEX; }
"vector"|"VECTOR"|"Vector"                  { printf("VECTOR "); return VECTOR; }
"matrix"|"MATRIX"|"Matrix"                  { printf("MATRIX "); return MATRIX; }
"texture"|"TEXTURE"|"Texture"               { printf("TEXTURE "); return TEXTURE; }
"model"|"MODEL"|"Model"                     { printf("MODEL "); return MODEL; }
"scene"|"SCENE"|"Scene"                     { printf("SCENE "); return SCENE; }
"camera"|"CAMERA"|"Camera"                  { printf("CAMERA "); return CAMERA; }
"index"|"INDEX"|"Index"                     { printf("INDEX "); return INDEX; }
"textcoord"|"TEXTCOORD"|"Textcoord"         { printf("TEXTCOORD "); return TEXTCOORD; }
"normal"|"NORMAL"|"Normal"                  { printf("NORMAL "); return NORMAL; }
"render"|"RENDER"|"Render"                  { printf("RENDER "); return RENDER; }
"triangles"|"TRIANGLES"|"Triangles"         { printf("TRIANGLES "); return TRIANGLES; }
"trianglestrips"|"TRIANGLESTRIPS"|"Trianglestrips"  { printf("TRIANGLESTRIPS "); return TRIANGLESTRIPS; }
"light"|"LIGHT"|"Light"                     { printf("LIGHT "); return LIGHT; }
"ambient"|"AMBIENT"|"Ambient"               { printf("AMBIENT "); return AMBIENT; }
"specular"|"SPECULAR"|"Specular"            { printf("SPECULAR "); return SPECULAR; }
"diffuse"|"DIFFUSE"|"Diffuse"               { printf("DIFFUSE "); return DIFFUSE; }
[ \t]                                       { /* ignore */ }
.                                           { /* ignore */ }


%%
