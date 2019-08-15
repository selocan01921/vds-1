﻿import React from 'react';
import { connect } from 'react-redux';

const Home = props => (
    <div>
    <h1>Добро пожаловать на сайт нашей компании!</h1>
    <p>Наша компания разрабатывает программные решения в областях:</p>
    <ul>
            <li>Системы распределённого реестра: хранение данных, криптовалюты и тому подобное.</li>
      <li>Системы безопасности: системы видеонаблюдения, программное обеспечение для умных домов и т.д. </li>
      <li>Управление разработкой программного обеспечения: формирование команд, проработка архитектуры, аудит деятельности и т.д.</li>
    </ul>
  </div>
);

export default connect()(Home);