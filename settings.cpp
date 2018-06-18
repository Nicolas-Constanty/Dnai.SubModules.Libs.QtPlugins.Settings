#include <QDebug>
#include <QGuiApplication>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>

#include "settings.h"

namespace dnai
{
	Settings::Settings(QObject *parent) :
        QObject(parent), m_format(QSettings::IniFormat)
		, m_parameters(nullptr)
	{
#ifdef QT_DEBUG
        m_settings.clear();
#endif
		initConnections();
    }

	void Settings::initConnections() const
	{
        connect(this, SIGNAL(settingFolderChanged(const QString&)),
                this, SLOT(parseFolder(const QString&)));
        connect(this, SIGNAL(themePathsChanged(QStringList)),
                this, SLOT(refreshThemes(const QStringList&)));
        connect(this, SIGNAL(parametersChanged(SettingsParameters *)),
                this, SLOT(refreshParameters(SettingsParameters *)));
        connect(this, SIGNAL(currentThemeChanged(const QString &)),
                this, SLOT(refreshTheme(const QString &)));
        connect(this, SIGNAL(currentThemeChanged(const QString &)),
                this, SLOT(refreshThemeLoaded(const QString &)));
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

    const QStringList& Settings::themePaths() const
	{
        return m_themePaths;
	}

    void Settings::setThemePaths(const QStringList& value)
	{
        if (m_themePaths == value)
			return;
        m_themePaths = value;
        emit themePathsChanged(value);
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
        if (m_themePaths.contains(value))
			return;
        m_themePaths.append(value);
        emit themeNamesChanged(m_themePaths);
	}

	void Settings::removeTheme(const QString& value)
	{
        if (!m_themePaths.contains(value))
			return;
        m_themePaths.removeOne(value);
        emit themeNamesChanged(m_themePaths);
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

    QSettings *Settings::settings()
    {
        return &m_settings;
    }

    bool Settings::themeLoaded() const
    {
        return !m_currentTheme.isEmpty();
    }

    QStringList Settings::themeNames() const
    {
        return QStringList(m_themes.keys());
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
                qDebug() << "Dnai.Settings 1.0 successfully loaded " << basename << "theme";
		}
	}

	void Settings::refreshParameters(SettingsParameters* parameters)
	{
		setFormat(parameters->format());
		setSettingFolder(parameters->settingFolder());
		setPrefix(parameters->prefix());
        setThemePaths(parameters->themePaths());
		setCurrentTheme(parameters->currentTheme());
	}

    bool Settings::themeAlreadySet()
    {
        return !m_settings.value(m_prefix + "/currentTheme").toString().isEmpty();
    }

    void Settings::restoreFromJson()
    {
        m_settings.clear();
        refreshThemes(m_themePaths);
    }

    void Settings::saveFromMap(const QString &name, const QVariantMap &value)
    {
        m_themes[name] = value;
        m_settings.setValue(m_prefix + "/themes/" + name, value);
    }

    void Settings::refreshTheme(const QString& theme)
    {
        m_theme = this->operator [](theme);
        m_settings.setValue(m_prefix + "/currentTheme", m_currentTheme);
        emit themeChanged(m_theme);
    }

    void Settings::refreshThemeLoaded(const QString &theme)
    {
        emit themeLoadedChanged(!theme.isEmpty());
    }

	bool Settings::loadJsonTheme(const QString &name, const QString &path)
	{
        if (m_settings.value(m_prefix + "/themes/" + name).isValid())
        {
            m_themes[name] = qvariant_cast<QVariantMap>(m_settings.value(m_prefix + "/themes/" + name).toMap());
            emit themesChanged(m_themes);
            emit themeNamesChanged(m_themes.keys());
        }
        else {
            QFile file(path);
            if (!file.open(QIODevice::ReadOnly)) {
                qWarning("Couldn't open file.");
                return false;
            }
            const auto data = file.readAll();

            try {
                QJsonParseError err;
                const auto obj(QJsonDocument::fromJson(data, &err).object());
                if (err.error != QJsonParseError::NoError)
                {
                    qWarning() << err.errorString() << "at line :" << err.offset;
                    return false;
                }
                m_themes[name] = obj.toVariantMap();
                m_settings.setValue(m_prefix + "/themes/" + name, m_themes[name]);
                emit themesChanged(m_themes);
                emit themeNamesChanged(m_themes.keys());
            }
            catch (std::exception &e) {
                Q_UNUSED(e)
                qWarning("Couldn't parse file.");
                file.close();
                return false;
            }
            file.close();
        }
		return true;
	}
}

