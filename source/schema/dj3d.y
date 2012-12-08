/* dj3d.y */

%{
    #include <stdio.h>

    extern char * yytext;

    void yyerror(char* errmsg);
    int yylex(void);
    int yyparse(void); 

    extern "C" void createScene(unsigned int id);
    extern "C" void submitMatrix(void);
    extern "C" void createCamera(float fovy, float znear, float zfar);
    extern "C" void submitCamera(void);
    extern "C" void createLight(unsigned int id, int bon);
    extern "C" void submitAmbient(void);
    extern "C" void submitSpecular(void);
    extern "C" void submitDiffuse(void);
    extern "C" void submitLightPosition(void);
    extern "C" void submitLightDirection(void);
    extern "C" void submitLight(void);
    extern "C" void createTexture(unsigned int id);
    extern "C" void submitTexture(void);
    extern "C" void createModel(unsigned int id, unsigned int attr);
    extern "C" void submitVertex(void);
    extern "C" void submitVector(void);
    extern "C" void createRenderUnit(unsigned int layer, unsigned int textid, unsigned int op);
    extern "C" void createTriangleList(unsigned int count);
    extern "C" void createTriangleStripList(unsigned int count);
    extern "C" void createIndexList(void);
    extern "C" void submitIndexList(void);
    extern "C" void createTextCoordList(void);
    extern "C" void submitTextCoordList(void);
    extern "C" void createNormalList(void);
    extern "C" void submitNormalList(void);
    extern "C" void submitTriangleList(void);
    extern "C" void submitTriangleStripList(void);
    extern "C" void submitRenderUnit(void);
    extern "C" void submitModel(void);
    extern "C" void submitScene(void);
    extern "C" void onFloat(float fVal);
    extern "C" void onUint(unsigned int uVal);
    extern "C" void onString(const char* str);
%}

%union
{
    float  f32;
    unsigned int u32;
    char* str;
}

%token<f32>  FLOAT
%token<u32>  UINT
%token<str>  STRING
%token  ID
%token  ATTR
%token  LAYER
%token  TEXTID
%token  OP
%token  COUNT
%token  FOVY
%token  ZN
%token  ZF
%token  LT
%token  GT
%token  LTS
%token  EQUAL
%token  COMMA
%token  VERTEX
%token  VECTOR
%token  MATRIX
%token  SCENE
%token  TEXTURE
%token  MODEL
%token  CAMERA
%token  INDEX
%token  TEXTCOORD
%token  NORMAL
%token  RENDER
%token  TRIANGLES
%token  TRIANGLESTRIPS
%token  LIGHT
%token  AMBIENT
%token  SPECULAR
%token  DIFFUSE
%token  STATUS
%token  ON
%token  OFF
%%

scene : scene_start camera light_list texture_list model_list scene_end { submitScene(); }
    ;
    
scene_start : LT SCENE ID EQUAL UINT GT { createScene($5);}
    ;

scene_end : LTS SCENE GT {}
    ;
    
camera : camera_start vertex vector vector camera_end { submitCamera(); }
    ;
 
camera_start : LT CAMERA FOVY EQUAL FLOAT ZN EQUAL FLOAT ZF EQUAL FLOAT GT { createCamera($5, $8, $11); }
    ;

camera_end : LTS CAMERA GT {}
    ;

light_list : light 
    | light_list light
    ;

light: light_start light_desc light_end { submitLight(); }
    ;

light_start : LT LIGHT ID EQUAL UINT STATUS EQUAL ON GT { createLight($5, 1); }
    | LT LIGHT ID EQUAL UINT STATUS EQUAL OFF GT { createLight($5, 0); }
    ;

light_desc : ambient specular diffuse light_position light_direction {}
    ;

ambient : LT AMBIENT GT float_list LTS AMBIENT GT { submitAmbient(); }
    ;

specular : LT SPECULAR GT float_list LTS SPECULAR GT { submitSpecular(); }
    ;

diffuse : LT DIFFUSE GT float_list LTS DIFFUSE GT { submitDiffuse(); }
    ;

light_position : vertex { submitLightPosition(); }
    ;
    
light_direction : vector { submitLightDirection(); }
    ;

light_end : LTS LIGHT GT {}
    ;

texture_list: texture
    | texture_list texture
    ;

texture: texture_start string texture_end { submitTexture(); }
    ;

texture_start: LT TEXTURE ID EQUAL UINT GT { createTexture($5); }
    ;
        
texture_end : LTS TEXTURE GT {}
    ;
    
model_list : model
    | model_list model
    ;
    
model: model_start model_content model_end { submitModel(); }
    ;

model_start : LT MODEL ID EQUAL UINT ATTR EQUAL UINT GT { createModel($5, $8); }
    ;

model_content : matrix vertex_list renderunit_list
    ;
    
model_end : LTS MODEL GT {}
    ;

renderunit_list : renderunit
    | renderunit_list renderunit
    ;

renderunit : renderunit_start shapes renderunit_end { submitRenderUnit(); }
    ;

renderunit_start : LT RENDER LAYER EQUAL UINT TEXTID EQUAL UINT OP EQUAL UINT GT { createRenderUnit($5, $8, $11); }
    ;
    
renderunit_end : LTS RENDER GT {}
    ;
    
shapes : triangle_list
    | trianglestrip_list
    | shapes triangle_list 
    | shapes trianglestrip_list
    ;

trianglestrip_list : trianglestrip_list_start index_list textcoord_list normal_list trianglestrip_list_end { submitTriangleStripList(); }
    ;

trianglestrip_list_start : LT TRIANGLESTRIPS GT { createTriangleStripList(0); }
    ;

trianglestrip_list_end : LTS TRIANGLESTRIPS GT {}
    ;
    
triangle_list: triangle_list_start index_list textcoord_list normal_list triangle_list_end { submitTriangleList(); }
    ;

triangle_list_start : LT TRIANGLES GT { createTriangleList(0); }
    ;

triangle_list_end : LTS TRIANGLES GT {}
    ;
    
index_list : index_list_start int_list index_list_end { submitIndexList(); }
    ;

index_list_start : LT INDEX GT { createIndexList(); }
    ;

index_list_end : LTS INDEX GT {}
    ;

textcoord_list : textcoord_list_start float_list textcoord_list_end { submitTextCoordList(); }
    ;

textcoord_list_start : LT TEXTCOORD GT { createTextCoordList(); }
    ;
    
textcoord_list_end : LTS TEXTCOORD GT {}
    ;

normal_list : normal_list_start vector_list normal_list_end { submitNormalList(); }
    ;

normal_list_start : LT NORMAL GT { createNormalList(); }
    ;

normal_list_end : LTS NORMAL GT {}
    ;
        
vector_list: vector
    | vector_list vector
    ;
    
vertex_list: vertex 
    | vertex_list vertex
    ;

vector: LT VECTOR GT float_list LTS VECTOR GT { submitVector(); }
    ;

vertex: LT VERTEX GT float_list LTS VERTEX GT { submitVertex(); }
    ;

matrix: LT MATRIX GT float_list LTS MATRIX GT { submitMatrix(); }
    ;

float_list : FLOAT { onFloat($1); }
    | float_list COMMA FLOAT { onFloat($3); } 
    ;

int_list : UINT { onUint($1); }
    | int_list COMMA UINT { onUint($3); }
    ;

string : STRING { onString($1); }
    ;

%%

void yyerror(char* errmsg)
{
    printf("shit, error : \"%s\"\n", errmsg);
    printf("yytext is \"%s\"\n", yytext);
}
