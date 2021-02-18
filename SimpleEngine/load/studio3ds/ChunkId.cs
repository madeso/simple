using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.load.studio3ds
{
    internal static class ChunkId
    {
        public const int MAIN_CHUNK = 0x4D4D;
        public const int EDITOR_3D_CHUNK = 0x3D3D;
        public const int OBJECT_BLOCK = 0x4000;
        public const int TRIANGULAR_MESH = 0x4100;
        public const int VERTICES_LIST = 0x4110;
        public const int FACES_DESCRIPTION = 0x4120;
        public const int FACES_MATERIAL = 0x4130;
        public const int MAPPING_COORDINATES_LIST = 0x4140;
        public const int SMOOTHING_GROUP_LIST = 0x4150;
        public const int LOCAL_COORDINATES_SYSTEM = 0x4160;
        public const int LIGHT = 0x4600;
        public const int SPOTLIGHT = 0x4610;
        public const int CAMERA = 0x4700;
        public const int MATERIAL_BLOCK = 0xAFFF;
        public const int MATERIAL_NAME = 0xA000;
        public const int AMBIENT_COLOR = 0xA010;
        public const int DIFFUSE_COLOR = 0xA020;
        public const int SPECULAR_COLOR = 0xA030;
        public const int TEXTURE_MAP_1 = 0xA200;
        public const int BUMP_MAP = 0xA230;
        public const int REFLECTION_MAP = 0xA220;
        //public const int [SUB CHUNKS FOR EACH MAP]
        public const int MAPPING_FILENAME = 0xA300;
        public const int MAPPING_PARAMETERS = 0xA351;
        public const int KEYFRAMER_CHUNK = 0xB000;
        public const int MESH_INFORMATION_BLOCK = 0xB002;
        public const int SPOT_LIGHT_INFORMATION_BLOCK = 0xB007;
        public const int FRAMES /*(START AND END)*/ = 0xB008;
        public const int OBJECT_NAME = 0xB010;
        public const int OBJECT_PIVOT_POINT = 0xB013;
        public const int POSITION_TRACK = 0xB020;
        public const int ROTATION_TRACK = 0xB021;
        public const int SCALE_TRACK = 0xB022;
        public const int HIERARCHY_POSITION = 0xB030;

        public static string ToString(int id)
        {
            switch(id)
            {
            case MAIN_CHUNK : return "MAIN_CHUNK";
            case EDITOR_3D_CHUNK : return "EDITOR_3D_CHUNK";
            case OBJECT_BLOCK : return "OBJECT_BLOCK";
            case TRIANGULAR_MESH : return "TRIANGULAR_MESH";
            case VERTICES_LIST : return "VERTICES_LIST";
            case FACES_DESCRIPTION : return "FACES_DESCRIPTION";
            case FACES_MATERIAL : return "FACES_MATERIAL";
            case MAPPING_COORDINATES_LIST : return "MAPPING_COORDINATES_LIST";
            case SMOOTHING_GROUP_LIST : return "SMOOTHING_GROUP_LIST";
            case LOCAL_COORDINATES_SYSTEM : return "LOCAL_COORDINATES_SYSTEM";
            case LIGHT : return "LIGHT";
            case SPOTLIGHT : return "SPOTLIGHT";
            case CAMERA : return "CAMERA";
            case MATERIAL_BLOCK : return "MATERIAL_BLOCK";
            case MATERIAL_NAME : return "MATERIAL_NAME";
            case AMBIENT_COLOR : return "AMBIENT_COLOR";
            case DIFFUSE_COLOR : return "DIFFUSE_COLOR";
            case SPECULAR_COLOR : return "SPECULAR_COLOR";
            case TEXTURE_MAP_1 : return "TEXTURE_MAP_1";
            case BUMP_MAP : return "BUMP_MAP";
            case REFLECTION_MAP : return "REFLECTION_MAP";
            case MAPPING_FILENAME : return "MAPPING_FILENAME";
            case MAPPING_PARAMETERS : return "MAPPING_PARAMETERS";
            case KEYFRAMER_CHUNK : return "KEYFRAMER_CHUNK";
            case MESH_INFORMATION_BLOCK : return "MESH_INFORMATION_BLOCK";
            case SPOT_LIGHT_INFORMATION_BLOCK : return "SPOT_LIGHT_INFORMATION_BLOCK";
            case FRAMES: return "FRAMES (START AND END)";
            case OBJECT_NAME : return "OBJECT_NAME";
            case OBJECT_PIVOT_POINT : return "OBJECT_PIVOT_POINT";
            case POSITION_TRACK : return "POSITION_TRACK";
            case ROTATION_TRACK : return "ROTATION_TRACK";
            case SCALE_TRACK : return "SCALE_TRACK";
            case HIERARCHY_POSITION : return "HIERARCHY_POSITION";
            default: return id.ToString("X");
            }
        }
    }
}
