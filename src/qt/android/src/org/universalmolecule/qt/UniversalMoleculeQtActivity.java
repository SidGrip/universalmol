package org.universalmolecule.qt;

import android.os.Bundle;

import org.qtproject.qt5.android.bindings.QtActivity;

import java.io.File;

public class UniversalMoleculeQtActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        final File universalMoleculeDir = new File(getFilesDir().getAbsolutePath() + "/.universalmolecule");
        if (!universalMoleculeDir.exists()) {
            universalMoleculeDir.mkdir();
        }

        super.onCreate(savedInstanceState);
    }
}
