/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

#include "ui/effects/premium_stars.h"

namespace Ui {
class RpWidget;

namespace Premium {

class ColoredMiniStars final {
public:
	// optimizeUpdate may cause paint glitch.
	ColoredMiniStars(not_null<Ui::RpWidget*> parent, bool optimizeUpdate);

	void setSize(const QSize &size);
	void setPosition(QPoint position);
	void setColorOverride(std::optional<QColor> color);
	void setCenter(const QRect &rect);
	void paint(QPainter &p);

	void setPaused(bool paused);

private:
	Ui::Premium::MiniStars _ministars;
	QRectF _ministarsRect;
	QImage _frame;
	QImage _mask;
	QSize _size;
	QPoint _position;
	std::optional<QColor> _colorOverride;

};

} // namespace Premium
} // namespace Ui
