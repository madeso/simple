#include <string>

namespace simple
{
    namespace math
    {
        // larger smoothing values result in a smoother but less responsive path
        float Curve(float new_value, float old_value, float smoothing_value);
        float ParseFloat(const std::string& text);
        bool IsZero(float l);

        float Square(float r);
        int Square(int r);

        float Sqrt(float r);

        float Abs(float r);

        float Min(float lhs, float rhs);

        float Max(float lhs, float rhs);

        float Limmit(float min, float value, float max);

        float To01(float min, float v, float max);
        float From01(float min, float v, float max);
        float Remap(float ol, float oor, float v, float nl, float nr);

        float Get360Angular(float min, float value, float max);

        float Within(float min, float v, float max);
        bool IsWithin(float min, float c, float max);

        float Wrap(float min, float v, float max);
        int Wrapi(int min, int v, int max);

        // return number of wraps (with sign)
        int IncrementAndWrap(float min, float* current, float change, float max);

        int IncrementAndWrapi(int min, int* current, int change, int max);

        bool IsBetween(int start, float p, int end);

        bool IsSame(float lhs, float rhs);

        float ZeroOrValue(float mark);

        constexpr float pi = 3.14159265359f;
    }
}
