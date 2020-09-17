/*
* Copyright (C) 2011 The Android Open Source Project
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include "glUtils.h"
#include <string.h>
#include "ErrorLog.h"
#include <IOStream.h>

#include <GLES3/gl31.h>

size_t glSizeof(GLenum type)
{
    size_t retval = 0;
    switch(type) {
    case GL_BYTE:
    case GL_UNSIGNED_BYTE:
        retval = 1;
        break;
    case GL_SHORT:
    case GL_UNSIGNED_SHORT:
    case GL_HALF_FLOAT:
    case GL_HALF_FLOAT_OES:
        retval = 2;
        break;
    case GL_IMAGE_2D:
    case GL_IMAGE_3D:
    case GL_UNSIGNED_INT:
    case GL_INT:
    case GL_FLOAT:
    case GL_FIXED:
    case GL_BOOL:
        retval =  4;
        break;
#ifdef GL_DOUBLE
    case GL_DOUBLE:
        retval = 8;
        break;
#endif
    case GL_FLOAT_VEC2:
    case GL_INT_VEC2:
    case GL_UNSIGNED_INT_VEC2:
    case GL_BOOL_VEC2:
        retval = 8;
        break;
    case GL_INT_VEC3:
    case GL_UNSIGNED_INT_VEC3:
    case GL_BOOL_VEC3:
    case GL_FLOAT_VEC3:
        retval = 12;
        break;
    case GL_FLOAT_VEC4:
    case GL_BOOL_VEC4:
    case GL_INT_VEC4:
    case GL_UNSIGNED_INT_VEC4:
    case GL_FLOAT_MAT2:
        retval = 16;
        break;
    case GL_FLOAT_MAT3:
        retval = 36;
        break;
    case GL_FLOAT_MAT4:
        retval = 64;
        break;
    case GL_FLOAT_MAT2x3:
    case GL_FLOAT_MAT3x2:
        retval = 4 * 6;
        break;
    case GL_FLOAT_MAT2x4:
    case GL_FLOAT_MAT4x2:
        retval = 4 * 8;
        break;
    case GL_FLOAT_MAT3x4:
    case GL_FLOAT_MAT4x3:
        retval = 4 * 12;
        break;
    case GL_SAMPLER_2D:
    case GL_SAMPLER_3D:
    case GL_SAMPLER_CUBE:
        retval = 4;
        break;
    case GL_UNSIGNED_SHORT_4_4_4_4:
	case GL_UNSIGNED_SHORT_5_5_5_1:
	case GL_UNSIGNED_SHORT_5_6_5:
	case GL_UNSIGNED_SHORT_4_4_4_4_REV_EXT:
	case GL_UNSIGNED_SHORT_1_5_5_5_REV_EXT:
        retval = 2;
        break;
	case GL_INT_2_10_10_10_REV:
	case GL_UNSIGNED_INT_10F_11F_11F_REV:
	case GL_UNSIGNED_INT_5_9_9_9_REV:
	case GL_UNSIGNED_INT_2_10_10_10_REV:
	case GL_UNSIGNED_INT_24_8_OES:;
        retval = 4;
        break;
	case GL_FLOAT_32_UNSIGNED_INT_24_8_REV:
		retval = 4 + 4;
        break;
    default:
        ERR("**** ERROR unknown type 0x%x (%s,%d)\n", type, __FUNCTION__,__LINE__);
        retval = 4;
    }
    return retval;

}

size_t glUtilsParamSize(GLenum param)
{
    size_t s = 0;

    switch(param)
    {
    case GL_DEPTH_TEST:
    case GL_DEPTH_FUNC:
    case GL_DEPTH_BITS:
    case GL_MAX_CLIP_PLANES:
    case GL_MAX_COLOR_ATTACHMENTS:
    case GL_MAX_DRAW_BUFFERS:
    case GL_GREEN_BITS:
    case GL_MAX_MODELVIEW_STACK_DEPTH:
    case GL_MAX_PROJECTION_STACK_DEPTH:
    case GL_MAX_TEXTURE_STACK_DEPTH:
    case GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES:
    case GL_IMPLEMENTATION_COLOR_READ_TYPE_OES:
    case GL_NUM_COMPRESSED_TEXTURE_FORMATS:
    case GL_MAX_TEXTURE_SIZE:
    case GL_TEXTURE_GEN_MODE_OES:
    case GL_TEXTURE_ENV_MODE:
    case GL_FOG_MODE:
    case GL_FOG_DENSITY:
    case GL_FOG_START:
    case GL_FOG_END:
    case GL_SPOT_EXPONENT:
    case GL_CONSTANT_ATTENUATION:
    case GL_LINEAR_ATTENUATION:
    case GL_QUADRATIC_ATTENUATION:
    case GL_SHININESS:
    case GL_LIGHT_MODEL_TWO_SIDE:
    case GL_POINT_SIZE:
    case GL_POINT_SIZE_MIN:
    case GL_POINT_SIZE_MAX:
    case GL_POINT_FADE_THRESHOLD_SIZE:
    case GL_CULL_FACE:
    case GL_CULL_FACE_MODE:
    case GL_FRONT_FACE:
    case GL_SHADE_MODEL:
    case GL_DEPTH_WRITEMASK:
    case GL_DEPTH_CLEAR_VALUE:
    case GL_STENCIL_FAIL:
    case GL_STENCIL_PASS_DEPTH_FAIL:
    case GL_STENCIL_PASS_DEPTH_PASS:
    case GL_STENCIL_REF:
    case GL_STENCIL_WRITEMASK:
    case GL_MATRIX_MODE:
    case GL_MODELVIEW_STACK_DEPTH:
    case GL_PROJECTION_STACK_DEPTH:
    case GL_TEXTURE_STACK_DEPTH:
    case GL_ALPHA_TEST_FUNC:
    case GL_ALPHA_TEST_REF:
    case GL_ALPHA_TEST:
    case GL_DITHER:
    case GL_BLEND_DST:
    case GL_BLEND_SRC:
    case GL_BLEND:
    case GL_LOGIC_OP_MODE:
    case GL_SCISSOR_TEST:
    case GL_MAX_TEXTURE_UNITS:
    case GL_ACTIVE_TEXTURE:
    case GL_ALPHA_BITS:
    case GL_ARRAY_BUFFER_BINDING:
    case GL_BLUE_BITS:
    case GL_CLIENT_ACTIVE_TEXTURE:
    case GL_CLIP_PLANE0:
    case GL_CLIP_PLANE1:
    case GL_CLIP_PLANE2:
    case GL_CLIP_PLANE3:
    case GL_CLIP_PLANE4:
    case GL_CLIP_PLANE5:
    case GL_COLOR_ARRAY:
    case GL_COLOR_ARRAY_BUFFER_BINDING:
    case GL_COLOR_ARRAY_SIZE:
    case GL_COLOR_ARRAY_STRIDE:
    case GL_COLOR_ARRAY_TYPE:
    case GL_COLOR_LOGIC_OP:
    case GL_COLOR_MATERIAL:
    case GL_PACK_ALIGNMENT:
    case GL_PERSPECTIVE_CORRECTION_HINT:
    case GL_POINT_SIZE_ARRAY_BUFFER_BINDING_OES:
    case GL_POINT_SIZE_ARRAY_STRIDE_OES:
    case GL_POINT_SIZE_ARRAY_TYPE_OES:
    case GL_POINT_SMOOTH:
    case GL_POINT_SMOOTH_HINT:
    case GL_POINT_SPRITE_OES:
    case GL_COORD_REPLACE_OES:
    case GL_COMBINE_ALPHA:
    case GL_SRC0_RGB:
    case GL_SRC1_RGB:
    case GL_SRC2_RGB:
    case GL_OPERAND0_RGB:
    case GL_OPERAND1_RGB:
    case GL_OPERAND2_RGB:
    case GL_SRC0_ALPHA:
    case GL_SRC1_ALPHA:
    case GL_SRC2_ALPHA:
    case GL_OPERAND0_ALPHA:
    case GL_OPERAND1_ALPHA:
    case GL_OPERAND2_ALPHA:
    case GL_RGB_SCALE:
    case GL_ALPHA_SCALE:
    case GL_COMBINE_RGB:
    case GL_POLYGON_OFFSET_FACTOR:
    case GL_POLYGON_OFFSET_FILL:
    case GL_POLYGON_OFFSET_UNITS:
    case GL_RED_BITS:
    case GL_RESCALE_NORMAL:
    case GL_SAMPLE_ALPHA_TO_COVERAGE:
    case GL_SAMPLE_ALPHA_TO_ONE:
    case GL_SAMPLE_BUFFERS:
    case GL_SAMPLE_COVERAGE:
    case GL_SAMPLE_COVERAGE_INVERT:
    case GL_SAMPLE_COVERAGE_VALUE:
    case GL_SAMPLES:
    case GL_MAX_SAMPLES_EXT:
    case GL_STENCIL_BITS:
    case GL_STENCIL_CLEAR_VALUE:
    case GL_STENCIL_FUNC:
    case GL_STENCIL_TEST:
    case GL_STENCIL_VALUE_MASK:
    case GL_STENCIL_BACK_FUNC:
    case GL_STENCIL_BACK_VALUE_MASK:
    case GL_STENCIL_BACK_REF:
    case GL_STENCIL_BACK_FAIL:
    case GL_STENCIL_BACK_PASS_DEPTH_FAIL:
    case GL_STENCIL_BACK_PASS_DEPTH_PASS:
    case GL_STENCIL_BACK_WRITEMASK:
    case GL_TEXTURE_2D:
    case GL_TEXTURE_BASE_LEVEL:
    case GL_TEXTURE_BINDING_2D:
    case GL_TEXTURE_BINDING_CUBE_MAP:
    case GL_TEXTURE_BINDING_EXTERNAL_OES:
    case GL_TEXTURE_COMPARE_FUNC:
    case GL_TEXTURE_COMPARE_MODE:
    case GL_TEXTURE_COORD_ARRAY:
    case GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING:
    case GL_TEXTURE_COORD_ARRAY_SIZE:
    case GL_TEXTURE_COORD_ARRAY_STRIDE:
    case GL_TEXTURE_COORD_ARRAY_TYPE:
    case GL_TEXTURE_IMMUTABLE_FORMAT:
    case GL_UNPACK_ALIGNMENT:
    case GL_VERTEX_ARRAY:
    case GL_VERTEX_ARRAY_BUFFER_BINDING:
    case GL_VERTEX_ARRAY_SIZE:
    case GL_VERTEX_ARRAY_STRIDE:
    case GL_VERTEX_ARRAY_TYPE:
    case GL_SPOT_CUTOFF:
    case GL_TEXTURE_MIN_FILTER:
    case GL_TEXTURE_MAG_FILTER:
    case GL_TEXTURE_MAX_LOD:
    case GL_TEXTURE_MIN_LOD:
    case GL_TEXTURE_WRAP_S:
    case GL_TEXTURE_WRAP_T:
    case GL_TEXTURE_WRAP_R:
    case GL_TEXTURE_SWIZZLE_R:
    case GL_TEXTURE_SWIZZLE_G:
    case GL_TEXTURE_SWIZZLE_B:
    case GL_TEXTURE_SWIZZLE_A:
    case GL_GENERATE_MIPMAP:
    case GL_GENERATE_MIPMAP_HINT:
    case GL_RENDERBUFFER_WIDTH_OES:
    case GL_RENDERBUFFER_HEIGHT_OES:
    case GL_RENDERBUFFER_INTERNAL_FORMAT_OES:
    case GL_RENDERBUFFER_RED_SIZE_OES:
    case GL_RENDERBUFFER_GREEN_SIZE_OES:
    case GL_RENDERBUFFER_BLUE_SIZE_OES:
    case GL_RENDERBUFFER_ALPHA_SIZE_OES:
    case GL_RENDERBUFFER_DEPTH_SIZE_OES:
    case GL_RENDERBUFFER_STENCIL_SIZE_OES:
    case GL_RENDERBUFFER_BINDING:
    case GL_FRAMEBUFFER_BINDING:
    case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_OES:
    case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_OES:
    case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_OES:
    case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_OES:
    case GL_FENCE_STATUS_NV:
    case GL_FENCE_CONDITION_NV:
    case GL_TEXTURE_WIDTH_QCOM:
    case GL_TEXTURE_HEIGHT_QCOM:
    case GL_TEXTURE_DEPTH_QCOM:
    case GL_TEXTURE_INTERNAL_FORMAT_QCOM:
    case GL_TEXTURE_FORMAT_QCOM:
    case GL_TEXTURE_TYPE_QCOM:
    case GL_TEXTURE_IMAGE_VALID_QCOM:
    case GL_TEXTURE_NUM_LEVELS_QCOM:
    case GL_TEXTURE_TARGET_QCOM:
    case GL_TEXTURE_OBJECT_VALID_QCOM:
    case GL_BLEND_EQUATION_RGB_OES:
    case GL_BLEND_EQUATION_ALPHA_OES:
    case GL_BLEND_DST_RGB_OES:
    case GL_BLEND_SRC_RGB_OES:
    case GL_BLEND_DST_ALPHA_OES:
    case GL_BLEND_SRC_ALPHA_OES:
    case GL_MAX_LIGHTS:
    case GL_SHADER_TYPE:
    case GL_COMPILE_STATUS:
    case GL_SHADER_SOURCE_LENGTH:
    case GL_CURRENT_PROGRAM:
    case GL_SUBPIXEL_BITS:
    case GL_MAX_3D_TEXTURE_SIZE:
    case GL_MAX_ARRAY_TEXTURE_LAYERS:
    case GL_MAX_CUBE_MAP_TEXTURE_SIZE:
    case GL_NUM_SHADER_BINARY_FORMATS:
    case GL_SHADER_COMPILER:
    case GL_MAX_VERTEX_ATTRIBS:
    case GL_MAX_VERTEX_UNIFORM_VECTORS:
    case GL_MAX_VARYING_VECTORS:
    case GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:
    case GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS:
    case GL_MAX_FRAGMENT_UNIFORM_VECTORS:
    case GL_MAX_RENDERBUFFER_SIZE:
    case GL_MAX_TEXTURE_IMAGE_UNITS:
    case GL_REQUIRED_TEXTURE_IMAGE_UNITS_OES:
    case GL_FRAGMENT_SHADER_DERIVATIVE_HINT_OES:
    case GL_LINE_WIDTH:
    case GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS:
    case GL_MAX_UNIFORM_BUFFER_BINDINGS:
    case GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS:
    case GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS:
    case GL_UNIFORM_BLOCK_BINDING:
    case GL_UNIFORM_BLOCK_DATA_SIZE:
    case GL_UNIFORM_BLOCK_NAME_LENGTH:
    case GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS:
    case GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER:
    case GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER:
    case GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT:
    case GL_CURRENT_QUERY:
    case GL_QUERY_RESULT:
    case GL_QUERY_RESULT_AVAILABLE:
    case GL_READ_BUFFER:
    case GL_NUM_PROGRAM_BINARY_FORMATS:
    case GL_PROGRAM_BINARY_FORMATS:

    case GL_ACTIVE_ATOMIC_COUNTER_BUFFERS:
    case GL_ACTIVE_ATTRIBUTES:
    case GL_ACTIVE_ATTRIBUTE_MAX_LENGTH:
    case GL_ACTIVE_UNIFORM_BLOCKS:
    case GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH:
    case GL_ACTIVE_UNIFORMS:
    case GL_ACTIVE_UNIFORM_MAX_LENGTH:
    case GL_ATTACHED_SHADERS:
    case GL_DELETE_STATUS:
    case GL_INFO_LOG_LENGTH:
    case GL_LINK_STATUS:
    case GL_PROGRAM_BINARY_LENGTH:
    case GL_PROGRAM_BINARY_RETRIEVABLE_HINT:
    case GL_PROGRAM_SEPARABLE:
    case GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT:
    case GL_TRANSFORM_FEEDBACK_BUFFER_MODE:
    case GL_TRANSFORM_FEEDBACK_VARYINGS:
    case GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH:
    case GL_TRANSFORM_FEEDBACK_ACTIVE:
#define GL_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
#define GL_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 0x8C80
    case GL_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS:
    case GL_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS:
    case GL_VALIDATE_STATUS:
        s = 1;
        break;
    case GL_ALIASED_LINE_WIDTH_RANGE:
    case GL_ALIASED_POINT_SIZE_RANGE:
    case GL_DEPTH_RANGE:
    case GL_MAX_VIEWPORT_DIMS:
    case GL_SMOOTH_POINT_SIZE_RANGE:
    case GL_SMOOTH_LINE_WIDTH_RANGE:
    case GL_SAMPLE_POSITION:
        s= 2;
        break;
    case GL_SPOT_DIRECTION:
    case GL_POINT_DISTANCE_ATTENUATION:
    case GL_CURRENT_NORMAL:
    case GL_COMPUTE_WORK_GROUP_SIZE:
        s =  3;
        break;
    case GL_CURRENT_VERTEX_ATTRIB:
    case GL_CURRENT_TEXTURE_COORDS:
    case GL_CURRENT_COLOR:
    case GL_FOG_COLOR:
    case GL_AMBIENT:
    case GL_DIFFUSE:
    case GL_SPECULAR:
    case GL_EMISSION:
    case GL_POSITION:
    case GL_LIGHT_MODEL_AMBIENT:
    case GL_TEXTURE_ENV_COLOR:
    case GL_SCISSOR_BOX:
    case GL_VIEWPORT:
    case GL_TEXTURE_CROP_RECT_OES:
    case GL_COLOR_CLEAR_VALUE:
    case GL_COLOR_WRITEMASK:
    case GL_AMBIENT_AND_DIFFUSE:
    case GL_BLEND_COLOR:
        s =  4;
        break;
    case GL_MODELVIEW_MATRIX:
    case GL_PROJECTION_MATRIX:
    case GL_TEXTURE_MATRIX:
        s = 16;
    break;
    default:
        s = 1; // assume 1 for everything else
    }
    return s;
}

void glUtilsPackPointerData(unsigned char *dst, unsigned char *src,
                     int size, GLenum type, unsigned int stride,
                     unsigned int datalen)
{
    unsigned int  vsize = size * glSizeof(type);
    switch (type) {
    case GL_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
        vsize = vsize / 4;
        break;
    default:
        break;
    }

    if (stride == 0) stride = vsize;

    if (stride == vsize) {
        memcpy(dst, src, datalen);
    } else {
        for (unsigned int i = 0; i < datalen; i += vsize) {
            memcpy(dst, src, vsize);
            dst += vsize;
            src += stride;
        }
    }
}


#ifndef GL_RGBA16F
#define GL_RGBA16F                        0x881A
#endif // GL_RGBA16F

int glUtilsPixelBitSize(GLenum format, GLenum type)
{
    int components = 0;
    int componentsize = 0;
    int pixelsize = 0;
    switch(type) {
    case GL_BYTE:
    case GL_UNSIGNED_BYTE:
        componentsize = 8;
        break;
    case GL_SHORT:
    case GL_HALF_FLOAT:
    case GL_UNSIGNED_SHORT:
        componentsize = 16;
        break;
    case GL_INT:
    case GL_UNSIGNED_INT:
    case GL_FLOAT:
    case GL_FIXED:
        componentsize = 32;
        break;
    case GL_UNSIGNED_SHORT_5_6_5:
    case GL_UNSIGNED_SHORT_4_4_4_4:
    case GL_UNSIGNED_SHORT_5_5_5_1:
    case GL_RGB565_OES:
    case GL_RGB5_A1_OES:
    case GL_RGBA4_OES:
        pixelsize = 16;
        break;
    case GL_UNSIGNED_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_24_8_OES:
        pixelsize = 32;
        break;
    default:
        ERR("glUtilsPixelBitSize: unknown pixel type %d - assuming pixel data 0\n", type);
        componentsize = 0;
    }

    if (pixelsize == 0) {
        switch(format) {
#if 0
        case GL_RED:
        case GL_GREEN:
        case GL_BLUE:
#endif
        case GL_ALPHA:
        case GL_LUMINANCE:
        case GL_DEPTH_COMPONENT:
        case GL_DEPTH_STENCIL_OES:
            components = 1;
            break;
        case GL_LUMINANCE_ALPHA:
            components = 2;
            break;
        case GL_RGB:
#if 0
        case GL_BGR:
#endif
            components = 3;
            break;
        case GL_RGBA:
        case GL_BGRA_EXT:
            components = 4;
            break;
        case GL_RGBA16F:
            pixelsize = 64;
            break;
        default:
            ERR("glUtilsPixelBitSize: unknown pixel format %d\n", format);
            components = 0;
        }
        if (pixelsize == 0) {
            pixelsize = components * componentsize;
        }
    }

    return pixelsize;
}

// pack a list of strings into one.
void glUtilsPackStrings(char *ptr,  char **strings,  GLint *length, GLsizei count)
{
    char *p = ptr;
    *p = '\0';
    for (int i = 0; i < count; i++) {
        int l=0;
        if (strings[i]!=NULL) {
            if (length == NULL || length[i] < 0) {
                l = strlen(strings[i]);
                strcat(p, strings[i]);
            } else {
                l = length[i];
                strncat(p, strings[i], l);
            }
        }
        p += l;
    }
}

// claculate the length of a list of strings
int glUtilsCalcShaderSourceLen( char **strings,  GLint *length, GLsizei count)
{
    int len = 0;
    for (int i = 0; i < count; i++) {
        int l;
        if (length == NULL || length[i] < 0) {
            l = strings[i]!=NULL ? strlen(strings[i]) : 0;
        } else {
            l = length[i];
        }
        len += l;
    }
    return len;

}

// helper to get GL_COLOR_ATTACHMENTn names
GLenum glUtilsColorAttachmentName(int i) {
#undef COLOR_ATTACHMENT_CASE
#define COLOR_ATTACHMENT_CASE(i) \
    case i: \
        return GL_COLOR_ATTACHMENT##i; \

    switch (i) {
        COLOR_ATTACHMENT_CASE(0)
        COLOR_ATTACHMENT_CASE(1)
        COLOR_ATTACHMENT_CASE(2)
        COLOR_ATTACHMENT_CASE(3)
        COLOR_ATTACHMENT_CASE(4)
        COLOR_ATTACHMENT_CASE(5)
        COLOR_ATTACHMENT_CASE(6)
        COLOR_ATTACHMENT_CASE(7)
        COLOR_ATTACHMENT_CASE(8)
        COLOR_ATTACHMENT_CASE(9)
        COLOR_ATTACHMENT_CASE(10)
        COLOR_ATTACHMENT_CASE(11)
        COLOR_ATTACHMENT_CASE(12)
        COLOR_ATTACHMENT_CASE(13)
        COLOR_ATTACHMENT_CASE(14)
        COLOR_ATTACHMENT_CASE(15)
    }
    return GL_NONE;
#undef COLOR_ATTACHMENT_CASE
}

int glUtilsColorAttachmentIndex(GLenum attachment) {
#undef COLOR_ATTACHMENT_CASE
#define COLOR_ATTACHMENT_CASE(i) \
    case GL_COLOR_ATTACHMENT##i: \
        return i; \

    switch (attachment) {
        COLOR_ATTACHMENT_CASE(0)
        COLOR_ATTACHMENT_CASE(1)
        COLOR_ATTACHMENT_CASE(2)
        COLOR_ATTACHMENT_CASE(3)
        COLOR_ATTACHMENT_CASE(4)
        COLOR_ATTACHMENT_CASE(5)
        COLOR_ATTACHMENT_CASE(6)
        COLOR_ATTACHMENT_CASE(7)
        COLOR_ATTACHMENT_CASE(8)
        COLOR_ATTACHMENT_CASE(9)
        COLOR_ATTACHMENT_CASE(10)
        COLOR_ATTACHMENT_CASE(11)
        COLOR_ATTACHMENT_CASE(12)
        COLOR_ATTACHMENT_CASE(13)
        COLOR_ATTACHMENT_CASE(14)
        COLOR_ATTACHMENT_CASE(15)
    }
    return -1;
#undef COLOR_ATTACHMENT_CASE
}

struct glUtilsDrawArraysIndirectCommand {
    GLuint count;
    GLuint primCount;
    GLuint first;
    GLuint reserved;
};

struct glUtilsDrawElementsIndirectCommand {
    GLuint count;
    GLuint primCount;
    GLuint first;
    GLint baseVertex;
    GLuint reserved;
};

GLuint glUtilsIndirectStructSize(IndirectCommandType cmdType) {
    switch (cmdType) {
    case INDIRECT_COMMAND_DRAWARRAYS:
        return sizeof(glUtilsDrawArraysIndirectCommand);
    case INDIRECT_COMMAND_DRAWELEMENTS:
        return sizeof(glUtilsDrawElementsIndirectCommand);
    }
    return 4;
}
