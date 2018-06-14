#ifndef SETTINGS_H
#define SETTINGS_H

#include <QStringList>

#include "settingsparameters.h"

namespace dnai
{
	class Settings : public QObject
	{
        Q_OBJECT
		Q_PROPERTY(QString settingFolder READ settingFolder WRITE setSettingFolder NOTIFY settingFolderChanged)
		Q_PROPERTY(QString prefix READ prefix WRITE setPrefix NOTIFY prefixChanged)
		Q_PROPERTY(QSettings::Format format READ format WRITE setFormat NOTIFY formatChanged)
		Q_PROPERTY(QStringList themeNames READ themeNames WRITE setThemeNames NOTIFY themeNamesChanged)
		Q_PROPERTY(QString currentTheme READ currentTheme WRITE setCurrentTheme NOTIFY currentThemeChanged)

		Q_PROPERTY(SettingsParameters *parameters READ parameters WRITE setParameters NOTIFY parametersChanged)
		Q_PROPERTY(QVariantMap themes READ themes WRITE setThemes NOTIFY themesChanged)
		Q_PROPERTY(QVariantMap theme READ theme NOTIFY themeChanged)

	public:
		explicit Settings(QObject* parent = nullptr);

		/**
		 * \brief Return the settingFolder property value
		 * \return settingFolder value
		 */
		const QString& settingFolder() const;

		/**
		 * \brief Set the settingFolder property value and refresh themes list if folder contain new file
		 * \param value 
		 */
		void setSettingFolder(const QString& value);

		/*
		 * Prefix property, allow to add extra token to the settings path
		 */
		const QString& prefix() const;
		void setPrefix(const QString& value);

		/*
		 * Format property, allow set internal m_settings format
		 */
		QSettings::Format format() const;
		void setFormat(QSettings::Format value);

		/*
		 * Themes property, the list of available themes installed
		 */
		const QStringList& themeNames() const;
		void setThemeNames(const QStringList& value);
		void addTheme(const QString& value);
		void removeTheme(const QString& value);

		const QString& currentTheme() const;
		void setCurrentTheme(const QString& value);

		SettingsParameters *parameters() const;
		void setParameters(SettingsParameters *value);

		const QVariantMap& themes() const;
		void setThemes(const QVariantMap& value);
		QVariantMap operator[](const QString& value) const;

		const QVariantMap &theme() const;

	signals :
		void settingFolderChanged(const QString& value);
		void prefixChanged(const QString& value);
		void formatChanged(QSettings::Format value);
		void themeNamesChanged(const QStringList& value);
		void parametersChanged(SettingsParameters *value);
		void themesChanged(const QVariantMap& value);
		void currentThemeChanged(const QString &value);
		void themeChanged(const QVariantMap &theme);

	public slots:
		void parseFolder(const QString& folder);
		void refreshThemes(const QStringList& themes);
		void refreshParameters(SettingsParameters *parameters);
		void refreshTheme(const QVariantMap &theme);

	private:
		void initConnections() const;
		bool loadJsonTheme(const QString &name, const QString& path);

	private:
		QString m_prefix;
		QSettings::Format m_format;
		QStringList m_themeNames;
		QVariantMap m_themes;
		QString m_currentTheme;
		SettingsParameters* m_parameters;
		QString m_settingFolder;
		QVariantMap m_theme;
	};
}

#endif // SETTINGS_H
