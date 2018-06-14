#include "dnaisettings_plugin.h"

#include <qqml.h>
#include <QFontDatabase>
#include "settings.h"

static QObject *settings_singleton_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new dnai::Settings();
}

void DnaiSettingsPlugin::registerTypes(const char *uri)
{
    // @uri Dnai.Settings
    const auto pluginUri = "Dnai.Settings";
    Q_ASSERT(uri == QLatin1String(pluginUri));
    qmlRegisterSingletonType<dnai::Settings>(pluginUri, 1, 0, "AppSettings", settings_singleton_provider);
    qmlRegisterType<dnai::SettingsParameters>(pluginUri, 1, 0, "SettingParameters");
}

