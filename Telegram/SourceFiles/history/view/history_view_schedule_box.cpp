/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#include "history/view/history_view_schedule_box.h"

#include "api/api_common.h"
#include "data/data_peer.h"
#include "data/data_user.h"
#include "lang/lang_keys.h"
#include "base/event_filter.h"
#include "base/qt/qt_key_modifiers.h"
#include "base/unixtime.h"
#include "ui/widgets/fields/input_field.h"
#include "ui/widgets/labels.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/popup_menu.h"
#include "ui/wrap/padding_wrap.h"
#include "menu/menu_send.h"
#include "styles/style_info.h"
#include "styles/style_layers.h"
#include "styles/style_chat.h"
#include "styles/style_menu_icons.h"

namespace HistoryView {
namespace {

void FillSendUntilOnlineMenu(
		not_null<Ui::IconButton*> button,
		Fn<void()> callback,
		const ScheduleBoxStyleArgs &style) {
	const auto menu = std::make_shared<base::unique_qptr<Ui::PopupMenu>>();
	button->setClickedCallback([=] {
		*menu = base::make_unique_q<Ui::PopupMenu>(
			button,
			*style.popupMenuStyle);
		(*menu)->addAction(
			tr::lng_scheduled_send_until_online(tr::now),
			std::move(callback),
			&st::menuIconWhenOnline);
		(*menu)->popup(QCursor::pos());
		return true;
	});
}

} // namespace

ScheduleBoxStyleArgs::ScheduleBoxStyleArgs()
: topButtonStyle(&st::infoTopBarMenu)
, popupMenuStyle(&st::popupMenuWithIcons)
, chooseDateTimeArgs({}) {
}

TimeId DefaultScheduleTime() {
	return base::unixtime::now() + 600;
}

bool CanScheduleUntilOnline(not_null<PeerData*> peer) {
	if (const auto user = peer->asUser()) {
		return !user->isSelf()
			&& !user->isBot()
			&& !user->lastseen().isHidden();
	}
	return false;
}

void ScheduleBox(
		not_null<Ui::GenericBox*> box,
		SendMenu::Type type,
		Fn<void(Api::SendOptions)> done,
		TimeId time,
		ScheduleBoxStyleArgs style) {
	const auto save = [=](bool silent, TimeId scheduleDate) {
		if (!scheduleDate) {
			return;
		}
		auto result = Api::SendOptions();
		// Pro tip: Hold Ctrl key to send a silent scheduled message!
		result.silent = silent || base::IsCtrlPressed();
		result.scheduled = scheduleDate;
		const auto copy = done;
		box->closeBox();
		copy(result);
	};
	auto descriptor = Ui::ChooseDateTimeBox(box, {
		.title = (type == SendMenu::Type::Reminder
			? tr::lng_remind_title()
			: tr::lng_schedule_title()),
		.submit = tr::lng_schedule_button(),
		.done = [=](TimeId result) { save(false, result); },
		.time = time,
		.style = style.chooseDateTimeArgs,
	});

	using T = SendMenu::Type;
	SendMenu::SetupMenuAndShortcuts(
		descriptor.submit.data(),
		[t = type == T::Disabled ? T::Disabled : T::SilentOnly] { return t; },
		[=] { save(true, descriptor.collect()); },
		nullptr,
		nullptr);

	if (type == SendMenu::Type::ScheduledToUser) {
		const auto sendUntilOnline = box->addTopButton(*style.topButtonStyle);
		const auto timestamp = Api::kScheduledUntilOnlineTimestamp;
		FillSendUntilOnlineMenu(
			sendUntilOnline.data(),
			[=] { save(false, timestamp); },
			style);
	}
}

} // namespace HistoryView
