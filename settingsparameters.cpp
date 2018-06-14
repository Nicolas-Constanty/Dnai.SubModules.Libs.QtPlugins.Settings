#include "settingsparameters.h"

namespace dnai
{
	SettingsParameters::SettingsParameters(QQuickItem* parent) :
		QQuickItem(parent), m_format(QSettings::NativeFormat)
	{
	}

	const QString& SettingsParameters::settingFolder() const
	{
		return m_settingFolder;
	}

	void SettingsParameters::setSettingFolder(const QString& value)
	{
		if (m_settingFolder == value)
			return;
		m_settingFolder = value;
        emit settingFolderChanged(value);
	}

	const QString& SettingsParameters::prefix() const
	{
		return m_prefix;
	}

	void SettingsParameters::setPrefix(const QString& value)
	{
		if (m_prefix == value)
			return;
		m_prefix = value;
		emit prefixChanged(value);
	}

	QSettings::Format SettingsParameters::format() const
	{
		return m_format;
	}

	void SettingsParameters::setFormat(QSettings::Format value)
	{
		if (m_format == value)
			return;
		m_format = value;
		emit formatChanged(value);
	}

	const QStringList& SettingsParameters::themeNames() const
	{
		return m_themeNames;
	}

	void SettingsParameters::setThemeNames(const QStringList& value)
	{
		if (m_themeNames == value)
			return;
		m_themeNames = value;
		emit themeNamesChanged(value);
	}

	const QString& SettingsParameters::currentTheme() const
	{
		return m_currentTheme;
	}

	void SettingsParameters::setCurrentTheme(const QString& value)
	{
		if (m_currentTheme == value)
			return;
		m_currentTheme = value;
		emit currentThemeChanged(value);
	}
}
