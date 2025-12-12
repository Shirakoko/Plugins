#pragma once
#include "UPlotData/UPlotDataBase.h"

#define COLOR_NODE_NORMAL_TEXT FLinearColor(0.7f,0.7f,0.7f)     // 一般的节点文本颜色
#define COLOR_NODE_MAIN_TEXT FLinearColor(0.5f, 0.7f, 1.f)		// 重要的节点文本颜色

constexpr float WIDTH_NODE = 250.f;	// 节点宽度

// 说话人文本样式
#define DIALOG_SPEAKER_TEXT_STYLE \
([]() -> const FTextBlockStyle& { \
    static FTextBlockStyle Style = [](){ \
        FTextBlockStyle S = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText"); \
        S.SetColorAndOpacity(FSlateColor(COLOR_NODE_MAIN_TEXT)); \
        S.SetFont(FCoreStyle::GetDefaultFontStyle("Regular", 10)); \
        return S; \
    }(); \
    return Style; \
}())

// 内容文本样式
#define DIALOG_CONTENT_TEXT_STYLE \
([]() -> const FTextBlockStyle& { \
    static FTextBlockStyle Style = [](){ \
        FTextBlockStyle S = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText"); \
        S.SetColorAndOpacity(FSlateColor(COLOR_NODE_NORMAL_TEXT)); \
        S.SetFont(FCoreStyle::GetDefaultFontStyle("Regular", 8)); \
        return S; \
    }(); \
    return Style; \
}())