#include <QDebug>
#include <QGuiApplication>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>

#include "settings.h"

namespace dnai
{
	Settings::Settings(QObject *parent) :
		QObject(parent), m_format(QSettings::NativeFormat)
		, m_parameters(nullptr)
	{
		initConnections();
    }

	void Settings::initConnections() const
	{
        connect(this, SIGNAL(settingFolderChanged()), this, SLOT(parseFolder()));
        connect(this, SIGNAL(themeNamesChanged()), this, SLOT(refreshThemes()));
        connect(this, SIGNAL(parametersChanged()), this, SLOT(refreshParameters()));
        connect(this, SIGNAL(currentThemeChanged()), this, SLOT(refreshTheme()));
	}

	const QString& Settings::settingFolder() const
	{
		return m_settingFolder;
	}

	void Settings::setSettingFolder(const QString& value)
	{
		if (m_settingFolder == value)
			return;
		m_settingFolder = value;
		emit settingFolderChanged(value);
	}

	const QString& Settings::prefix() const
	{
		return m_prefix;
	}

	void Settings::setPrefix(const QString& value)
	{
		if (m_prefix == value)
			return;
		m_prefix = value;
		emit prefixChanged(value);
	}

	QSettings::Format Settings::format() const
	{
		return m_format;
	}

	void Settings::setFormat(const QSettings::Format value)
	{
		if (m_format == value)
			return;
		m_format = value;
		emit formatChanged(value);
	}

	const QStringList& Settings::themeNames() const
	{
		return m_themeNames;
	}

	void Settings::setThemeNames(const QStringList& value)
	{
		if (m_themeNames == value)
			return;
		m_themeNames = value;
		emit themeNamesChanged(value);
	}

	const QString& Settings::currentTheme() const
	{
		return m_currentTheme;
	}

	void Settings::setCurrentTheme(const QString& value)
	{
		if (m_currentTheme == value)
			return;
		m_currentTheme = value;
		emit currentThemeChanged(value);
	}

	void Settings::addTheme(const QString& value)
	{
		if (m_themeNames.contains(value))
			return;
		m_themeNames.append(value);
		emit themeNamesChanged(m_themeNames);
	}

	void Settings::removeTheme(const QString& value)
	{
		if (!m_themeNames.contains(value))
			return;
		m_themeNames.removeOne(value);
		emit themeNamesChanged(m_themeNames);
	}

	SettingsParameters* Settings::parameters() const
	{
		return m_parameters;
	}

	void Settings::setParameters(SettingsParameters* value)
	{
		if (m_parameters == value)
			return;
		m_parameters = value;
		emit parametersChanged(value);
	}

	const QVariantMap& Settings::themes() const
	{
		return m_themes;
	}

	void Settings::setThemes(const QVariantMap& value)
	{
		if (m_themes == value)
			return;
		m_themes = value;
		emit themesChanged(value);
	}

	QVariantMap Settings::operator[](const QString& value) const
	{
		return qvariant_cast<QVariantMap>(m_themes[value]);
	}

	const QVariantMap& Settings::theme() const
	{
		return m_theme;
	}

	void Settings::parseFolder(const QString &folder)
	{
		if (folder.isEmpty())
			return;
		const auto path = QGuiApplication::applicationDirPath() + "/" + folder;
		QDir dir(path);
		const auto themeFilenames = dir.entryList(QDir::Files);
		for (const auto &filename : themeFilenames)
		{
			const auto f = QFileInfo(filename);
			if (f.suffix() == "json")
			{
				addTheme(filename);
			}
		}
	}

	void Settings::refreshThemes(const QStringList &themes)
	{
		for (const auto &theme : themes)
		{
			if (m_themes.contains(theme))
				continue;
			const auto f = QFileInfo(theme);
			const auto basename = f.baseName();
			if (f.suffix() != "json" || !loadJsonTheme(basename, theme))
			{
				qWarning() << "Cannot load" << basename << "with path :" << theme;
			}
			else
				qDebug() << "Theme \"" << basename << "\" successfully loaded.";
		}
	}

	void Settings::refreshParameters(SettingsParameters* parameters)
	{
		setFormat(parameters->format());
		setSettingFolder(parameters->settingFolder());
		setPrefix(parameters->prefix());
		setThemeNames(parameters->themeNames());
		setCurrentTheme(parameters->currentTheme());
	}

	void Settings::refreshTheme(const QVariantMap& theme)
	{
		m_theme = theme;
	}

	bool Settings::loadJsonTheme(const QString &name, const QString &path)
	{
		QFile file(path);
		if (!file.open(QIODevice::ReadOnly)) {
			qWarning("Couldn't open file.");
			return false;
		}
		const auto data = file.readAll();

		try {
			const auto obj(QJsonDocument::fromJson(data).object());
			m_themes[name] = obj.toVariantMap();
			emit themesChanged(m_themes);
		}
		catch (std::exception &e) {
			Q_UNUSED(e)
			qWarning("Couldn't parse file.");
			file.close();
			return false;
		}
		file.close();
		return true;
	}
}

