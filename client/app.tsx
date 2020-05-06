import ReactDOM from 'react-dom';
import React from 'react';

import './app.scss';
import PresetList from '#/Component/PresetList';

const root = document.getElementsByTagName('main')[0]!;
ReactDOM.render(<PresetList />, root);
