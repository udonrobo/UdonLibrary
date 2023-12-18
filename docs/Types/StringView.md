# StringView

文字列ビュー

## 個別インクルード

```cpp
#include <Udon/Types/StringView.hpp>
```

## 概要

文字列を保持せず、参照するクラスです。`std::string` と異なり、ヒープアロケーションが発生しないため効率の良い文字列操作を行えます。
