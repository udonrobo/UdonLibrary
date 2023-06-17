namespace udon
{
    constexpr int Ceil(double x)
    {
        return x - (int)x > 0 ? (int)x + 1 : (int)x;
    }
}    // namespace udon