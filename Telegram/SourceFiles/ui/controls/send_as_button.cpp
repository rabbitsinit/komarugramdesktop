/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#include "ui/controls/send_as_button.h"

#include "rabbit/settings/rabbit_settings.h"

#include "ui/effects/cross_animation.h"
#include "ui/painter.h"
#include "styles/style_chat.h"

namespace Ui {

SendAsButton::SendAsButton(QWidget *parent, const style::SendAsButton &st)
: AbstractButton(parent)
, _st(st) {
	resize(_st.width, _st.height);
}

void SendAsButton::setUserpic(QImage userpic) {
	_userpic = std::move(userpic);
	update();
}

void SendAsButton::setActive(bool active) {
	if (_active == active) {
		return;
	}
	_active = active;
	_activeAnimation.start(
		[=] { update(); },
		_active ? 0. : 1.,
		_active ? 1. : 0.,
		_st.duration);
}

void SendAsButton::paintEvent(QPaintEvent *e) {
	auto p = QPainter(this);

	const auto left = (width() - _st.size) / 2;
	const auto top = (height() - _st.size) / 2;

	const auto active = _activeAnimation.value(_active ? 1. : 0.);
	if (active < 1. && !_userpic.isNull()) {
		p.drawImage(QRect(left, top, _st.size, _st.size), _userpic);
	}
	if (active > 0.) {
		p.setOpacity(active);

		p.setPen(Qt::NoPen);
		p.setBrush(_st.activeBg);
		{
			PainterHighQualityEnabler hq(p);
			// p.drawEllipse(left, top, _st.size, _st.size);
			auto radius = _st.size * (RabbitSettings::JsonSettings::GetInt("userpic_roundness") / 100);
			p.drawRoundedRect(left, top, _st.size, _st.size, radius, radius);
		}

		CrossAnimation::paint(
			p,
			_st.cross,
			_st.activeFg,
			left,
			top,
			width(),
			active);
	}
}

} // namespace Ui
