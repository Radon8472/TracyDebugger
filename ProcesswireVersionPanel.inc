<?php

use Tracy\Debugger;
use Tracy\IBarPanel;

class ProcesswireVersionPanel implements IBarPanel {

    protected $icon;
    protected $versions;

    public function getTab() {
        if(\TracyDebugger::additionalBar()) return;
        \Tracy\Debugger::timer('processwireVersion');

        $rootFolder = function_exists('\ProcessWire\wire') ? \ProcessWire\wire('config')->paths->root : wire('config')->paths->root;
        $this->versions = array();
        foreach (new DirectoryIterator($rootFolder) as $fileInfo) {
            if($fileInfo->isDot()) continue;
            $filePath = $fileInfo->getPathname();
            $version = str_replace(wire('config')->paths->root, '', $filePath);
            $version = str_replace('.wire-', '', $version);
            if(strpos($filePath, 'wire') === false) continue;
            if(strpos($version, '-') !== false) continue;
            if(strpos($filePath, wire('config')->version) !== false) continue;
            if($version == 'wire') $version = wire('config')->version;
            $this->versions[] = $version;
        }
        usort($this->versions, 'version_compare');
        $latestVersion = array_pop((array_slice($this->versions, -1)));

        if($latestVersion == wire('config')->version) {
            $iconColor = '#009900';
        }
        else {
            $iconColor = '#FF9933';
        }

        $this->icon = '
            <svg version="1.1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" x="0px" y="0px" width="16px" height="16px" viewBox="312 504 16 16" enable-background="new 312 504 16 16" xml:space="preserve">
            <path d="M324.8,506.3c-1.8,0-3.2,1-3.2,2.3c0,0.8,0.6,1.6,1.6,2v0.3c0,0,0,2.3-3.2,2.3c-1.3,0-2.4,0.2-3.2,0.5v-5.4
                c1-0.4,1.6-1.1,1.6-2c0-1.3-1.4-2.3-3.2-2.3c-1.8,0-3.2,1-3.2,2.3c0,0.8,0.6,1.6,1.6,2v7.5c-1,0.4-1.6,1.1-1.6,2
                c0,1.3,1.4,2.3,3.2,2.3c1.8,0,3.2-1,3.2-2.3c0-0.6-0.3-1.1-0.9-1.5c0.5-0.4,1.2-0.7,2.5-0.7c6.4,0,6.4-4.6,6.4-4.6v-0.3
                c1-0.4,1.6-1.1,1.6-2C328,507.3,326.6,506.3,324.8,506.3z M315.2,505.1c0.9,0,1.6,0.5,1.6,1.1c0,0.6-0.7,1.1-1.6,1.1
                c-0.9,0-1.6-0.5-1.6-1.1C313.6,505.7,314.3,505.1,315.2,505.1z M315.2,518.9c-0.9,0-1.6-0.5-1.6-1.1c0-0.6,0.7-1.1,1.6-1.1
                c0.9,0,1.6,0.5,1.6,1.1C316.8,518.3,316.1,518.9,315.2,518.9z M324.8,509.7c-0.9,0-1.6-0.5-1.6-1.1c0-0.6,0.7-1.1,1.6-1.1
                c0.9,0,1.6,0.5,1.6,1.1C326.4,509.2,325.7,509.7,324.8,509.7z" fill="'.$iconColor.'" />
            </svg>
        ';

        return '
        <span title="ProcessWire Version">
            ' . $this->icon .   (\TracyDebugger::getDataValue('showPanelLabels') ? 'PW Version' : '') . ' ' . wire('config')->version . '
        </span>
        ';
    }


    public function getPanel() {

        $out = '
        <h1>' . $this->icon . ' ProcessWire Version</h1>

        <div class="tracy-inner">
            <fieldset>
                <legend>Choose from available versions</legend><br />';
                $out .= '
                <form method="post" action="./">
                    <select name="tracyPwVersion">';
                    foreach($this->versions as $version) {
                        $out .= '<option value="'.$version.'"'.($version == wire('config')->version ? 'selected="selected"' : '').'>'.$version.'</option>';
                    }
                    $out .= '
                    </select>&nbsp;<input type="submit" value="Change" />
                </form>
            </fieldset>';
            \TracyDebugger::$panelGenerationTime['processwireVersion'] = \Tracy\Debugger::timer('processwireVersion');
            $out .= \TracyDebugger::generatedTime(\TracyDebugger::$panelGenerationTime['processwireVersion']);
        $out .= '
        </div>';

        return $out;
    }

}