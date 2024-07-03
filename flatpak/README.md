# Sequencer FlatPak

This is a *proof of concept* for the packaging of the *CODAC* *COA* **Sequencer** GUI as a [FlatPak](https://www.flatpak.org).

This contains the manifest for the sequencer-gui, the sequencer *plugins*, and all the *COA* dependencies required to build them (including epics-base + epics-pvxs), the end target is a distributable build of the **Sequencer** GUI, with the sequencer plugins as optional extensions, all other *COA* manifests are included as baked in dependencies.

The runtime used is org.kde.Platform, because it's provided out of the box and includes the required Qt dependencies.

All manifests refer to the latest tagged version/release, *not* the latest commit.

## Usage

The FlaPak includes desktop files for all relevant GUI applications, meaning new entries for them should appear on your start menu upon installation.

Running the FlatPak directly with `flatpak run` invokes the sequencer *CLI*, not the graphical interface.

## Future improvements

All required dependencies are included/built directly or indirectly by the top level manifests, this proves troublesome in some cases, for instance a dependency that is used by 2 distinct plugins, but not the main application, which means that unless we choose to include a dependency that is not required and may not be used in the main application, it will be built and distributed twice (see sup-epics).

A better approach may be to also build a custom *CODAC*/*COA* runtime, with an extension point for these dependencies.

Some aspects of the current manifests are hardcoded for *Linux x86_64* and need to be updated to adjust dynamically to correct architecture.

Distribution through a public repository or the hosting of a dedicated one needs to be investigated.

## Structure
```
 .
 ├── org.iter.sequencer.yml
 ├── org.iter.sequencer.plugin.control.yml
 ├── org.iter.sequencer.plugin.epics.yml
 ├── org.iter.sequencer.plugin.mathexpr.yml
 ├── org.iter.sequencer.plugin.sup.yml
 ├── patches
 ├── desktop-files
 └── shared-modules
```

 - org.iter.sequencer.yml:

    Top level manifest to build the sequencer and sequencer-gui, with an extension point for the plugins.

 - org.iter.sequencer.plugin.control.yml:

    Top level manifest to build sequencer-plugin-control extension.

 - org.iter.sequencer.plugin.epics.yml:

    Top level manifest to build sequencer-plugin-control extension.

 - org.iter.sequencer.plugin.mathexpr.yml:

    Top level manifest to build sequencer-plugin-control extension.

 - org.iter.sequencer.plugin.sup.yml:

    Top level manifest to build sequencer-plugin-control extension.

 - patches:

    Patches needed for sources used directly in top level manifests.

 - desktop-files:

    Prebuilt desktop files for the applications provided. Arbitrarily separated as a stand-alone module included by the top manifest, could be added verbatim inline in the manifest modules instead.

    The projects include some desktop files, but they are not installed in the correct location for FlatPak nor are they strictly correct for its needs.

 - shared-modules

    modules/dependencies used by multiple *COA* projects or as indirect dependencies. Separated to standalone subdirectories to keep the main manifests simpler easier to read, this was an arbitrary decision, and the content could easily be nested in the main manifests instead. The name is shared with the FlatPak "shared-modules" concept but is unrelated besides using the same mechanisms.

## Building the FlatPak

To build the FlatPak you need access to the [COA repositories](https://git.iter.org/projects/COA), FlatPak and FlatPak Builder installed, and the runtime installed through FlatPak.

### Requirements

 - [FlatPak](https://github.com/flatpak/flatpak)
 - [FlatPak Builder](https://github.com/flatpak/flatpak-builder)
 - org.kde.Platform Runtime

Add the FlatHub remote if not already:
`flatpak remote-add --if-not-exists --user flathub https://dl.flathub.org/repo/flathub.flatpakrepo`

Install the runtime and sdk:
`flatpak install org.kde.Platform//5.15-23.08`
`flatpak install org.kde.Sdk//5.15-23.08`

### Building

Building the FlatPak can be done with:
`flatpak-builder --force-clean <builddir> <manifest>`

This will build the artifacts for `<manifest>` into `<builddir>`, note that this also produces a `.flatpak-builder` directory, this is where the builder caches sources and creates temporary build dirs, these are implementation details.

Additionally, you can build a local repository, later needed to create a distributable bundle instead of installing locally directly:
`flatpak-builder --force-clean --repo=<repo> <builddir> <manifest>`

This will create a local repository under the `<repo>` subdirectory.

Build the sequencer and all plugins (run these if you want to build the distributable files, if you just want to install locally run the ones under **Installing locally** instead):

`flatpak-builder --repo=local_repo --force-clean build/sequencer org.iter.sequencer.yml`

`flatpak-builder --repo=local_repo --force-clean build/sequencer-plugin-control org.iter.sequencer.plugin.control.yml`

`flatpak-builder --repo=local_repo --force-clean build/sequencer-plugin-mathexpr org.iter.sequencer.plugin.mathexpr.yml`

`flatpak-builder --repo=local_repo --force-clean build/sequencer-plugin-epics org.iter.sequencer.plugin.epics.yml`

`flatpak-builder --repo=local_repo --force-clean build/sequencer-plugin-sup org.iter.sequencer.plugin.sup.yml`

#### Installing locally

You can build and install the FlatPak locally directly, this is the most convenient option to test both the build and the resulting application:

Build and install the sequencer and all plugins:

`flatpak-builder --user --install --force-clean build/sequencer org.iter.sequencer.yml`

`flatpak-builder --user --install --force-clean build/sequencer-plugin-control org.iter.sequencer.plugin.control.yml`

`flatpak-builder --user --install --force-clean build/sequencer-plugin-mathexpr org.iter.sequencer.plugin.mathexpr.yml`

`flatpak-builder --user --install --force-clean build/sequencer-plugin-epics org.iter.sequencer.plugin.epics.yml`

`flatpak-builder --user --install --force-clean build/sequencer-plugin-sup org.iter.sequencer.plugin.sup.yml`

#### Distributable

You can create a single file bundle to share the application, see [flatpak/single-file-bundles](https://docs.flatpak.org/en/latest/single-file-bundles.html).

Create a distributable bundle for sequencer and all plugins:

`flatpak build-bundle local_repo sequencer.flatpak org.iter.sequencer --runtime-repo=https://flathub.org/repo/flathub.flatpakrepo`

`flatpak build-bundle local_repo sequencer-plugin-control.flatpak org.iter.sequencer.plugin.control --runtime`

`flatpak build-bundle local_repo sequencer-plugin-mathexpr.flatpak org.iter.sequencer.plugin.mathexpr --runtime`

`flatpak build-bundle local_repo sequencer-plugin-epics.flatpak org.iter.sequencer.plugin.epics --runtime`

`flatpak build-bundle local_repo sequencer-plugin-sup.flatpak org.iter.sequencer.plugin.sup --runtime`

These can later be directly installed with:

`flatpak install --user <bundle>`

### Troubleshooting

The manifests currently contain some hardcoded values for *Linux x86_64* machines and will very likely fail when building on different architectures.

The FlatPak builder makes many git requesting while preparing the sources, they may cause issues with the ITER network blocking external requests, if the sources have already been downloaded you can use the `--disable-download` flag to prevent this. If pulling the sources the first time proves difficult, comment out some of the sources in the manifest, this will cause the build to fail but the sources to be cached.

## Updating

Upon a release the manifests need to be updated, the extent of which should be:

 - The updating of the tag or commit on the `sources` entry on modules.
 - The addition of new patches needed or removal of defunct ones.

## Distributing

The purpose of the FlatPak is to have a platform independent distributable that can be easily installed by the consumers, building the FlatPak locally defeats this purpose... It is possible to build and publish the FlatPak to a repository, public or private, no effort was made in this direction. Please refer to the [FlatPak documentation](https://docs.flatpak.org/en/latest/hosting-a-repository.html).
