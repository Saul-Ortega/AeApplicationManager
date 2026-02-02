#include "filterproxymodel.h"
#include "applicationmodel.h"

FilterProxyModel::FilterProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent)
    , mOnlyFavorites()
    , mShowInstalled()
    , mFilterText()
{
}

QVariant FilterProxyModel::data(const QModelIndex& index, int role) const
{
    if ( !index.isValid() ) {
        return QVariant();
    }

    //MAPEA EL ÍNDICE DEL PROXY AL ÍNDICE DEL SOURCE
    QModelIndex sourceIndex = mapToSource(index);

    //DEVOLVEMOS EL VALOR SEGUN EL ROL SOLICITADO
    switch ( role ) {
    case ApplicationModel::IdRole :
        return sourceModel()->data(sourceIndex, ApplicationModel::IdRole);
    case ApplicationModel::NameRole :
    case Qt::DisplayRole :
        return sourceModel()->data(sourceIndex, ApplicationModel::NameRole);
    case ApplicationModel::DescriptionRole :
        return sourceModel()->data(sourceIndex, ApplicationModel::DescriptionRole);
    case ApplicationModel::ImageUrlRole :
        return sourceModel()->data(sourceIndex, ApplicationModel::ImageUrlRole);
    case ApplicationModel::ExecutableFileRole :
        return sourceModel()->data(sourceIndex, ApplicationModel::ExecutableFileRole);
    case ApplicationModel::ExpirationDateRole :
        return sourceModel()->data(sourceIndex, ApplicationModel::ExpirationDateRole);
    case ApplicationModel::IsLikedRole :
        return sourceModel()->data(sourceIndex, ApplicationModel::IsLikedRole);
    case ApplicationModel::UpdateRole :
        return sourceModel()->data(sourceIndex, ApplicationModel::UpdateRole);
    case ApplicationModel::IsDownloadedRole :
        return sourceModel()->data(sourceIndex, ApplicationModel::IsDownloadedRole);
    case ApplicationModel::VersionsRole :
        return sourceModel()->data(sourceIndex, ApplicationModel::VersionsRole);
    default :
        return QVariant();
    }
}

void FilterProxyModel::setSourceModel(QAbstractItemModel* sourceModel)
{
    QSortFilterProxyModel::setSourceModel(sourceModel);

    if ( !sourceModel ) {
        return;
    }
}

ApplicationModel* FilterProxyModel::applicationModel() const {
    return static_cast<ApplicationModel*>(sourceModel());
}

//
void FilterProxyModel::setFilterText(const QString& text)
{
    //GUARDAMOS EL TEXTO PARA EL FILTRADO
    mFilterText = text;

    //LLAMA A "filterAcceptsRow" PARA APLICAR LOS FILTROS Y VER SI MUESTRA LAS APPS
    invalidateFilter();
}

void FilterProxyModel::setShowOnlyFavorites(bool show)
{
    //GUARDAMOS SI QUIERE MOSTRAR LOS FAVORITOS O NO
    mOnlyFavorites = show;

    //LLAMA A "filterAcceptsRow" PARA APLICAR LOS FILTROS Y VER SI MUESTRA LAS APPS
    invalidateFilter();
}

void FilterProxyModel::setShowInstalled(bool show)
{
    //SI QUIERE MOSTRAR LAS APPS INSTALADAS SERA TRUE SINO FALSE
    mShowInstalled = show;

    //LLAMA A "filterAcceptsRow" PARA APLICAR LOS FILTROS Y VER SI MUESTRA LAS APPS
    invalidateFilter();
}

bool FilterProxyModel::filterAcceptsRow(int row, const QModelIndex& parent) const
{
    QModelIndex index = sourceModel()->index(row, 0, parent);

    bool isDownloaded = sourceModel()->data(index, ApplicationModel::IsDownloadedRole).toBool();
    bool isLiked = sourceModel()->data(index, ApplicationModel::IsLikedRole).toBool();
    QString name = sourceModel()->data(index, ApplicationModel::NameRole).toString();

    // === FILTRAR INSTALADAS / NO INSTALADAS ===
    //SI QUIERE MOSTRAR INSTALADOS Y LA APP NO ESTA INSTALADA DEVUELVE FALSE PARA NO MOSTRARLO
    if (mShowInstalled && !isDownloaded) {
        return false;
    }else{
    if (!mShowInstalled && isDownloaded){
        return false;
        }
    }

    // === FILTRAR FAVORITOS ===
    //SI PONE MOSTRAR FAVORITOS Y LA APP NO ES FAVORITA DEVUELVE FALSE PARA NO MOSTRARLO
    if (mOnlyFavorites && !isLiked){
        return false;
    }

    // FILTRAR POR TEXTO
    //SI INPUT TIENE ALGO Y EL NOMBRE DE LA APP NO TIENE COINCIDENCIAS DEVUELVE FALSE Y NO LO MUESTRA
    if (!mFilterText.isEmpty() &&
        !name.contains(mFilterText, Qt::CaseInsensitive)){
        return false;
    }

    //SI LA APP PASA TODOS LOS FILTROS ENTONCES SI SE MUESTRA
    return true;
}
