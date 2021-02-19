using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load.studio3ds
{
    namespace ChunkId
    {
        const int MAIN_CHUNK = 0x4D4D;
        const int EDITOR_3D_CHUNK = 0x3D3D;
        const int OBJECT_BLOCK = 0x4000;
        const int TRIANGULAR_MESH = 0x4100;
        const int VERTICES_LIST = 0x4110;
        const int FACES_DESCRIPTION = 0x4120;
        const int FACES_MATERIAL = 0x4130;
        const int MAPPING_COORDINATES_LIST = 0x4140;
        const int SMOOTHING_GROUP_LIST = 0x4150;
        const int LOCAL_COORDINATES_SYSTEM = 0x4160;
        const int LIGHT = 0x4600;
        const int SPOTLIGHT = 0x4610;
        const int CAMERA = 0x4700;
        const int MATERIAL_BLOCK = 0xAFFF;
        const int MATERIAL_NAME = 0xA000;
        const int AMBIENT_COLOR = 0xA010;
        const int DIFFUSE_COLOR = 0xA020;
        const int SPECULAR_COLOR = 0xA030;
        const int TEXTURE_MAP_1 = 0xA200;
        const int BUMP_MAP = 0xA230;
        const int REFLECTION_MAP = 0xA220;
        //const int [SUB CHUNKS FOR EACH MAP]
        const int MAPPING_FILENAME = 0xA300;
        const int MAPPING_PARAMETERS = 0xA351;
        const int KEYFRAMER_CHUNK = 0xB000;
        const int MESH_INFORMATION_BLOCK = 0xB002;
        const int SPOT_LIGHT_INFORMATION_BLOCK = 0xB007;
        const int FRAMES /*(START AND END)*/ = 0xB008;
        const int OBJECT_NAME = 0xB010;
        const int OBJECT_PIVOT_POINT = 0xB013;
        const int POSITION_TRACK = 0xB020;
        const int ROTATION_TRACK = 0xB021;
        const int SCALE_TRACK = 0xB022;
        const int HIERARCHY_POSITION = 0xB030;

        static std::string ToString(int id)
        {
            switch (id)
            {
            case MAIN_CHUNK:
                return "MAIN_CHUNK";
            case EDITOR_3D_CHUNK:
                return "EDITOR_3D_CHUNK";
            case OBJECT_BLOCK:
                return "OBJECT_BLOCK";
            case TRIANGULAR_MESH:
                return "TRIANGULAR_MESH";
            case VERTICES_LIST:
                return "VERTICES_LIST";
            case FACES_DESCRIPTION:
                return "FACES_DESCRIPTION";
            case FACES_MATERIAL:
                return "FACES_MATERIAL";
            case MAPPING_COORDINATES_LIST:
                return "MAPPING_COORDINATES_LIST";
            case SMOOTHING_GROUP_LIST:
                return "SMOOTHING_GROUP_LIST";
            case LOCAL_COORDINATES_SYSTEM:
                return "LOCAL_COORDINATES_SYSTEM";
            case LIGHT:
                return "LIGHT";
            case SPOTLIGHT:
                return "SPOTLIGHT";
            case CAMERA:
                return "CAMERA";
            case MATERIAL_BLOCK:
                return "MATERIAL_BLOCK";
            case MATERIAL_NAME:
                return "MATERIAL_NAME";
            case AMBIENT_COLOR:
                return "AMBIENT_COLOR";
            case DIFFUSE_COLOR:
                return "DIFFUSE_COLOR";
            case SPECULAR_COLOR:
                return "SPECULAR_COLOR";
            case TEXTURE_MAP_1:
                return "TEXTURE_MAP_1";
            case BUMP_MAP:
                return "BUMP_MAP";
            case REFLECTION_MAP:
                return "REFLECTION_MAP";
            case MAPPING_FILENAME:
                return "MAPPING_FILENAME";
            case MAPPING_PARAMETERS:
                return "MAPPING_PARAMETERS";
            case KEYFRAMER_CHUNK:
                return "KEYFRAMER_CHUNK";
            case MESH_INFORMATION_BLOCK:
                return "MESH_INFORMATION_BLOCK";
            case SPOT_LIGHT_INFORMATION_BLOCK:
                return "SPOT_LIGHT_INFORMATION_BLOCK";
            case FRAMES:
                return "FRAMES (START AND END)";
            case OBJECT_NAME:
                return "OBJECT_NAME";
            case OBJECT_PIVOT_POINT:
                return "OBJECT_PIVOT_POINT";
            case POSITION_TRACK:
                return "POSITION_TRACK";
            case ROTATION_TRACK:
                return "ROTATION_TRACK";
            case SCALE_TRACK:
                return "SCALE_TRACK";
            case HIERARCHY_POSITION:
                return "HIERARCHY_POSITION";
            default:
                return id.ToString("X");
            }
        }
    }
}
