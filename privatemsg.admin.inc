<?php
// $Id$

/**
 * @file
 * Administration menu callbacks for Privatemsg.
 */

function privatemsg_admin_settings() {
  $form = array();

  $form['theming_settings'] = array(
    '#type'        => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
    '#title'       => t('Theming settings'),
  );
  $form['theming_settings']['private_message_view_template'] = array(
    '#type'          => 'radios',
    '#title'         => t('Private message display template'),
    '#default_value' => variable_get('private_message_view_template', 'privatemsg-view'),
    '#options'       => private_message_view_options(),
  );
  $form['privatemsg_display_loginmessage'] = array(
    '#type' => 'checkbox',
    '#title' => t('Inform the user about new messages on login'),
    '#default_value' => variable_get('privatemsg_display_loginmessage', TRUE),
    '#description' => t('This option can safely be disabled if the "New message indication" block is used instead.'),
    '#weight'      => -5,
  );

  $form['integration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Integration settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('These settings allow to configure the integration with other modules.'),
  );
  $form['integration']['privatemsg_popups'] = array(
    '#type' => 'checkbox',
    '#title' => t('Open "send private message" links in a popup'),
    '#default_value' => variable_get('privatemsg_popups', TRUE),
    '#disabled' => !module_exists('popups'),
    '#description' => t('This requires the <a href="http://drupal.org/project/popups">Popups API module</a>. Also, the "Scan all pages for popup links." option needs to be enabled at <a href="!popups_admin_url">Popups Settings</a>.', array('!popups_admin_url' => url('admin/settings/popups'))),
  );

  $form['flush_deleted'] = array(
    '#type'        => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
    '#title'       => t('Flush deleted messages'),
    '#description' => t('By default, deleted messages are only hidden from the user but still stored in the database. These settings control if and when messages should be removed.'),
  );

  $form['flush_deleted']['privatemsg_flush_enabled'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Flush deleted messages'),
    '#default_value' => variable_get('privatemsg_flush_enabled', FALSE),
    '#description'   => t('Enable the flushing of deleted messages. Requires that cron is enabled'),
  );

  $form['flush_deleted']['privatemsg_flush_days'] = array(
    '#type' => 'select',
    '#title' => t('Flush messages after they have been deleted for more days than'),
    '#default_value' => variable_get('privatemsg_flush_days', 30),
    '#options' => drupal_map_assoc(array(0, 1, 2, 5, 10, 30, 100)),
  );

  $form['flush_deleted']['privatemsg_flush_max'] = array(
    '#type' => 'select',
    '#title' => t('Maximum number of messages to flush per cron run'),
    '#default_value' => variable_get('privatemsg_flush_max', 200),
    '#options' => drupal_map_assoc(array(50, 100, 200, 500, 1000)),
  );

  $form['privatemsg_listing'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configure listings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['privatemsg_listing']['privatemsg_per_page'] = array(
    '#type' => 'select',
    '#title' => t('Threads per page'),
    '#default_value' => variable_get('privatemsg_per_page', 25),
    '#options' => drupal_map_assoc(array(10, 25, 50, 75, 100)),
    '#description' => t('Choose the number of conversations that should be listed per page.'),
  );

  $form['privatemsg_listing']['privatemsg_display_fields'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Configure fields'),
    '#description' => t('Select which columns/fields should be displayed in the message listings. Subject and Last updated cannot be disabled.'),
    '#options' => array(
      'participants'    => t('Participants'),
      'thread_started'  => t('Started'),
      'count'           => t('Messages'),
    ),
    '#default_value' => variable_get('privatemsg_display_fields', array('participants')),
  );

  $amounts = drupal_map_assoc(array(5, 10, 20, 30, 50, 70, 90, 150, 200, 250, 300));
  $form['privatemsg_listing']['privatemsg_view_max_amount'] = array(
    '#type'          => 'select',
    '#title'         => t('Number of messages on thread pages'),
    '#options'       => $amounts + array(PRIVATEMSG_UNLIMITED => t('Unlimited')),
    '#default_value' => variable_get('privatemsg_view_max_amount', 20),
    '#description'   => t('Threads will not show more than this number of messages on a single page.'),
    '#weight'        => 10,
  );

  $form['privatemsg_listing']['privatemsg_view_use_max_as_default'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Display different amount of messages on first thread page'),
    '#default_value' => variable_get('privatemsg_view_use_max_as_default', FALSE),
    '#description'   => t('By default, the first thread page shows the maximally allowed amount of messages. Enable this checkbox to set a different value.'),
    '#weight'        => 15,
  );

  $form['privatemsg_listing']['privatemsg_view_default_amount'] = array(
    '#prefix'        => '<div id="privatemsg-view-default-button">',
    '#suffix'        => '</div>',
    '#type'          => 'select',
    '#title'         => t('Number of messages on first thread page'),
    '#default_value' => variable_get('privatemsg_view_default_amount', 10),
    '#description'   => t('The number of messages to be displayed on first thread page. Displays the newest messages.'),
    '#options'       => $amounts,
    '#weight'        => 20,
  );

  $form['links'] = array(
    '#type' => 'fieldset',
    '#title' => t('"Send private message" link settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['links']['privatemsg_display_link_self'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display "Send this user a message" links for themself'),
    '#description' => t('If enabled, each users sees that link on their own profile, comments and similiar places.'),
    '#default_value' => variable_get('privatemsg_display_link_self', TRUE),
    '#weight'        => -10,
  );

  $form['links']['privatemsg_display_profile_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display link on profile pages.'),
    '#description' => t('If this setting is enabled, a link to send a private message will be displayed.'),
    '#default_value' => variable_get('privatemsg_display_profile_links', 1),
  );

  $node_types = node_get_types('names');
  $form['links']['privatemsg_link_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display link on the selected content types'),
    '#description' => t('Select which content types should display a link to send a private message to the author. By default, the link is not displayed below teasers.'),
    '#default_value' => variable_get('privatemsg_link_node_types', array()),
    '#options' => $node_types,
  );
  $form['links']['privatemsg_display_on_teaser'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display link on teasers of the selected content types.'),
    '#default_value' => variable_get('privatemsg_display_on_teaser', 1),
  );
  $form['links']['privatemsg_display_on_comments'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display links on comments of selected content types.'),
    '#description' => t('Also display a link to send a private message to the authors of the comments of the selected content types.'),
    '#default_value' => variable_get('privatemsg_display_on_comments', 0),
  );
  drupal_add_js(drupal_get_path('module', 'privatemsg') .'/privatemsg-admin.js');

  $form['#submit'][] = 'privatemsg_admin_settings_submit';
  return system_settings_form($form);
}

function privatemsg_admin_settings_submit() {
  drupal_rebuild_theme_registry();
}