/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

#include "history/view/media/history_view_media.h"

namespace Data {
enum class CallFinishReason : char;
struct Call;
} // namespace Data

namespace HistoryView {

class Call : public Media {
public:
	Call(
		not_null<Element*> parent,
		not_null<Data::Call*> call);

	void draw(Painter &p, const PaintContext &context) const override;
	TextState textState(QPoint point, StateRequest request) const override;

	bool toggleSelectionByHandlerClick(const ClickHandlerPtr &p) const override {
		return true;
	}
	bool dragItemByHandler(const ClickHandlerPtr &p) const override {
		return false;
	}

	bool needsBubble() const override {
		return true;
	}
	bool customInfoLayout() const override {
		return true;
	}

private:
	using FinishReason = Data::CallFinishReason;

	QSize countOptimalSize() override;

	const int _duration = 0;
	const FinishReason _reason;
	const bool _video = false;

	QString _text;
	QString _status;

	ClickHandlerPtr _link;

};

} // namespace HistoryView
