#ifndef DNAI_SETTINGS_PARAMETERS_H
#define DNAI_SETTINGS_PARAMETERS_H

#include <QQuickItem>
#include <QSettings>

namespace dnai
{
	class SettingsParameters : public QQuickItem
	{
		Q_OBJECT
        Q_PROPERTY(QString settingFolder READ settingFolder WRITE setSettingFolder NOTIFY settingFolderChanged)
		Q_PROPERTY(QString prefix READ prefix WRITE setPrefix NOTIFY prefixChanged)
		Q_PROPERTY(QSettings::Format format READ format WRITE setFormat NOTIFY formatChanged)
		Q_PROPERTY(QStringList themeNames READ themeNames WRITE setThemeNames NOTIFY themeNamesChanged)
		Q_PROPERTY(QString currentTheme READ currentTheme WRITE setCurrentTheme NOTIFY currentThemeChanged)
	
	public:
		explicit SettingsParameters(QQuickItem *parent = nullptr);

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

		const QString& currentTheme() const;
		void setCurrentTheme(const QString& value);

	signals:
        void settingFolderChanged(const QString& value);
		void prefixChanged(const QString& value);
		void formatChanged(QSettings::Format value);
		void themeNamesChanged(const QStringList& value);
		void currentThemeChanged(const QString& value);

	private:
		QString m_settingFolder;
		QString m_prefix;
		QSettings::Format m_format;
		QStringList m_themeNames;
		QString m_currentTheme;
	};
}

#endif //DNAI_SETTINGS_PARAMETERS_H
